// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SNonPlayerCharacter.h"
#include "Animations/SAnimInstance.h"
#include "Characters/SRPGCharacter.h"
#include "Components/SStatComponent.h"
#include "Components/SWidgetComponent.h"
#include "Controllers/SAIController.h"
#include "Engine/DamageEvents.h"
#include "Game/SPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/SW_HPBar.h"


// Sets default values
ASNonPlayerCharacter::ASNonPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	WidgetComponent = CreateDefaultSubobject<USWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(300.f, 100.f));
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(!IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());

	if(IsValid(AnimInstance) && !AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::OnAttackAnimMontageEnded))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackAnimMontageEnded);
	}
}

float ASNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(StatComponent->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		if(IsValid(LastHitBy))
		{
			ASCharacter* DamageCauserCharacter = Cast<ASCharacter>(LastHitBy->GetPawn());
			if(IsValid(DamageCauserCharacter))
			{
				ASPlayerState* SPlayerState = Cast<ASPlayerState>(DamageCauserCharacter->GetPlayerState());
				if (IsValid(SPlayerState))
				{
					SPlayerState->SetCurrentEXP(SPlayerState->GetCurrentEXP() + 20.f);
				}
			}
		}

		ASAIController* AIController = Cast<ASAIController>(GetController());
		if(IsValid(AIController))
		{
			AIController->EndAI();
		}
	}
	
	return FinalDamageAmount;
}

void ASNonPlayerCharacter::SetWidget(USUserWidget* InUserWidget)
{
	Super::SetWidget(InUserWidget);

	USW_HPBar* HPBar = Cast<USW_HPBar>(InUserWidget);
	if(IsValid(HPBar))
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("[NPC] HP : %f"), StatComponent->GetCurrentHP()));
		HPBar->SetMaxHP(StatComponent->GetMaxHP());
		HPBar->InitializeHPBarWidget(StatComponent);
		StatComponent->OnCurrentHPChangeDelegate.AddDynamic(HPBar, &USW_HPBar::OnCurrentHPChange);
	}
}

void ASNonPlayerCharacter::Attack()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
	   HitResult,
	   GetActorLocation(),
	   GetActorLocation() + AttackRange,
	   FQuat::Identity,
	   ECollisionChannel::ECC_EngineTraceChannel2,
	   FCollisionShape::MakeSphere(AttackRadius),
	   Params
   );

	if(bResult && IsValid(HitResult.GetActor()))
	{
		ASCharacter* PlayerCharacter = Cast<ASCharacter>(HitResult.GetActor());
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->TakeDamage(10.f, FDamageEvent(),
				GetController(), this);
		}
	}

	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if(IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackMontage();
		bIsNowAttacking = true;
		if(!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::OnAttackAnimMontageEnded))
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnAttackAnimMontageEnded);
		}
	}

#pragma region CollisionDebugDrawing
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = true == bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.f;

	DrawDebugCapsule(
		GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);
#pragma endregion
}

void ASNonPlayerCharacter::OnAttackAnimMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	bIsNowAttacking = false;
}



