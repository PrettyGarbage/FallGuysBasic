// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/Enemy/EnemyBase.h"

#include "AIController.h"
#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/HUD/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->SightRadius = 4000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetHealthPercentage(AttributeComponent->GetHealthPercent());
		HealthBarWidget->SetVisibility(false);
	}

	AIController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);

	if(IsValid(PawnSensingComponent))
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyBase::PawnSeen);
	}
}

void AEnemyBase::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) && IsValid(HitReactMontage))
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemyBase::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) && IsValid(DeathMontage))
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(1, 4);
		const FName SectionName = FName(*FString::Printf(TEXT("Death%d"), Selection));
		DeathPose = GetDeathPoseEnumValue(SectionName);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->SetVisibility(false);
	}
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.f);
}

double AEnemyBase::GetTargetDistance() const
{
	if(!IsValid(CombatTarget)) return DBL_MAX;
	
	return FVector::Dist(GetActorLocation(), CombatTarget->GetActorLocation()); 
}

bool AEnemyBase::IsTargetInRange(AActor* InTarget, double Radius)
{
	if(!IsValid(InTarget)) return false;
	const double Distance = FVector::Distance(InTarget->GetActorLocation(), GetActorLocation());
	DrawDebugSphere(GetWorld(), InTarget->GetActorLocation(), Radius, 12, FColor::Red, false, 0.1f);
	return Distance <= Radius;
}

void AEnemyBase::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}


void AEnemyBase::CheckCombatTarget()
{
	if(!IsTargetInRange(CombatTarget, DistanceToTarget))
	{
		CombatTarget = nullptr;
		if(IsValid(HealthBarWidget))
		{
			HealthBarWidget->SetVisibility(false);
		}
	}
}

void AEnemyBase::CheckPatrolTarget()
{
	if(IsTargetInRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::PatrolTimerFinished, WaitTime);
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckCombatTarget();
	CheckPatrolTarget();
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::DirectionHitReact(const FVector& ImpactPoint)
{
	//내적을 이용하여 피격방향을 구한다.
	const FVector Forward = GetActorForwardVector();
	//높이에 대한 부분은 무시한다.
	const FVector ImpactHeight = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	//normalize를 통해 각도를 구할 수 맞은 방향의 방향 벡터를 구한다.
	const FVector ToHit = (ImpactHeight - GetActorLocation()).GetSafeNormal();
	//Forward * ToHit = |Forward| * |ToHit| * cos(각도)
	//|Forward| = 1, |ToHit| = 1이므로(두 단위벡터가 평행하면 절대값은 1이다) cos(각도) = Forward * ToHit
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	//acos를 통해 각도만 남기도록 한다.
	double Theta = FMath::Acos(CosTheta);
	//라디안을 각도로 변환한다.
	Theta = FMath::RadiansToDegrees(Theta);

	//오른손 법칙을 이용한 보정 코사인 함수가 주기함수라 방향이 잘못나오기도 한다고 함...
	const FVector Cross = FVector::CrossProduct(Forward, ToHit);

	if(Cross.Z < 0)
	{
		Theta *= -1.f;
	}

	FName SectionName;

	if(Theta >= -45.f && Theta < 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if(Theta >= -135.f && Theta < -45.f)
	{
		SectionName = FName("FromLeft");
	}
	else if(Theta >= 45.f && Theta < 135.f)
	{
		SectionName = FName("FromRight");
	}
	else
	{
		SectionName = FName("FromBack");
	}
	
	PlayHitReactMontage(SectionName);

	// 디버그용
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ Cross * 100.f,5.f, FColor::Red, 5.f, 1.f);
	//
	// if(IsValid(GEngine))
	// {
	// 	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta : %f"), Theta));
	// }
	//
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ Forward * 60.f,5.f, FColor::Red, 5.f, 1.f);
	// UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation()+ ToHit * 60.f,5.f, FColor::Red, 5.f, 1.f);
}

void AEnemyBase::GetHit_Implementation(const FVector& ImpactPoint)
{
	//첫 타격시에 UI가 노출되도록
	if(IsValid(HealthBarWidget) && !HealthBarWidget->IsVisible())
	{
		HealthBarWidget->SetVisibility(true);
	}
	
	if(IsValid(AttributeComponent) && AttributeComponent->IsAlive())
	{
		DirectionHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	

	if(IsValid(HitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	//const라서 IsValid가 안됨.
	if(HitParticle && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			ImpactPoint
		);
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(IsValid(AttributeComponent) && IsValid(HealthBarWidget))
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercentage(AttributeComponent->GetHealthPercent());
	}

	if(IsValid(EventInstigator))
	{
		CombatTarget = EventInstigator->GetPawn();
	}
	
	return DamageAmount;
}

EDeathPose AEnemyBase::GetDeathPoseEnumValue(FName InName)
{
	if(!DeathPoseMap.Contains(InName))
	{
		UE_LOG(LogTemp, Error, TEXT("DeathPoseMap does not contain %s"), *InName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DeathPoseMap contains %s"), *InName.ToString());
	}
	
	return DeathPoseMap.Contains(InName) ? DeathPoseMap[InName] : EDeathPose::EDP_Death1;
}

TObjectPtr<AActor> AEnemyBase::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for(auto Target : PatrolTargets)
	{
		if(Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
			
	const int32 NumPatrolTargets = ValidTargets.Num();
	if(NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemyBase::PawnSeen(APawn* SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Pawn Seen !!!"));
}

void AEnemyBase::MoveToTarget(AActor* InTarget)
{
	if(!IsValid(AIController) || !IsValid(InTarget)) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(InTarget);
	MoveRequest.SetAcceptanceRadius(15.f);
	AIController->MoveTo(MoveRequest);
}



