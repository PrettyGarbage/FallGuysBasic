// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Characters/STPSCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TPSStudyProject/Components/SStatComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPSStudyProject/Inputs/SInputConfigData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "TPSStudyProject/WorldStatic/SLandMine.h"


// Sets default values
ASTPSCharacter::ASTPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("SCharacter"));

	CameraComponent->SetRelativeLocation(FVector(0.f, 60.f, 20.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMeshComponent"));

	TimeBetweenFire = 60.f / FirePerMinute; //초당 발사 횟수 공격 속도라고 보면 됨.
}

void ASTPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(IsValid(Subsystem))
		{
			Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
		}
	}

	FName EquipmentSocketName = FName(TEXT("EquipmentSocket"));
	if(GetMesh()->DoesSocketExist(EquipmentSocketName))
	{
		WeaponSkeletalMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquipmentSocketName);
	}
}

void ASTPSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	CurrentFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaSeconds, 35.f);
	CameraComponent->SetFieldOfView(CurrentFOV);

	if(IsValid(GetController()))
	{
		FRotator ControlRotation = GetController()->GetControlRotation();
		CurrentAimPitch = ControlRotation.Pitch;
		CurrentAimYaw = ControlRotation.Yaw;
	}

	if(bIsNowRagdollBlending)
	{
		CurrentRagdollBlendWeight = FMath::FInterpTo(CurrentRagdollBlendWeight, TargetRagdollBlendWeight, DeltaSeconds, 10.f);

		FName PivotBoneName = FName(TEXT("spine_01"));
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PivotBoneName, CurrentRagdollBlendWeight);

		if(CurrentRagdollBlendWeight - TargetRagdollBlendWeight < KINDA_SMALL_NUMBER)
		{
			bIsNowRagdollBlending = false;
		}

		if(IsValid(GetStatComponent()) && GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
		{
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName(TEXT("root")), 1.f);
			GetMesh()->SetSimulatePhysics(true);
			bIsNowRagdollBlending = false;
		}
	}

	if(IsValid(GetController()))
	{
		PreviousAimPitch = CurrentAimPitch;
		PreviousAimYaw = CurrentAimYaw;

		FRotator ControlRotation = GetController()->GetControlRotation();
		CurrentAimPitch = ControlRotation.Pitch;
		CurrentAimYaw = ControlRotation.Yaw;

		if(PreviousAimPitch != CurrentAimPitch || PreviousAimYaw != CurrentAimYaw)
		{
			if(!HasAuthority())//서버 제외하고 업데이트
			{
				UpdateAimValue_Server(CurrentAimPitch, CurrentAimYaw);
			}
		}
	}

	if(PreviousForwardInputValue != ForwardInputValue || PreviousRightInputValue != RightInputValue)
	{
		if(!HasAuthority())
		{
			UpdateInputValue_Server(ForwardInputValue, RightInputValue);
		}
	}
}

float ASTPSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	PlayRagdoll_NetMulticast();

	return ActualDamage;
}

void ASTPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ForwardInputValue);
	DOREPLIFETIME(ThisClass, RightInputValue);
	DOREPLIFETIME(ThisClass, CurrentAimPitch);
	DOREPLIFETIME(ThisClass, CurrentAimYaw);
}

void ASTPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &ThisClass::Attack);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->IronSightAction, ETriggerEvent::Started, this, &ThisClass::StartIronSight);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->IronSightAction, ETriggerEvent::Completed, this, &ThisClass::EndIronSight);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->TriggerAction, ETriggerEvent::Started, this, &ThisClass::ToggleTrigger);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->AttackAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfigData->LandMineAction, ETriggerEvent::Started, this, &ThisClass::SpawnLandMine);
	}
}

void ASTPSCharacter::Move(const FInputActionValue& InValue)
{
	FVector2d MovementVector = InValue.Get<FVector2d>();
	ForwardInputValue = MovementVector.X;
	RightInputValue = MovementVector.Y;

	const FRotator CurrentControlRotation = GetController()->GetControlRotation();
	const FRotator CurrentControlRotationYaw(0.f, CurrentControlRotation.Yaw, 0.f);

	FVector ForwardDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(CurrentControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASTPSCharacter::Look(const FInputActionValue& InValue)
{
	FVector2d LookAxisVector = InValue.Get<FVector2d>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASTPSCharacter::Attack(const FInputActionValue& InValue)
{
	if(!bIsTriggerToggle)
	{
		Fire();
	}
	
}

void ASTPSCharacter::Fire()
{
	if(HasAuthority() || GetOwner() != UGameplayStatics::GetPlayerController(this, 0))
	{
		return;
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(!IsValid(PlayerController))
	{
		//플레이어 컨트롤러가 유효하지 않다면 공격이 실행될 수 없음.
		return;
	}

	FHitResult HitResult;

	FVector CameraStartLocation = CameraComponent->GetComponentLocation();
	FVector CameraEndLocation = CameraStartLocation + CameraComponent->GetForwardVector() * 5000.f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredComponent(static_cast<const UPrimitiveComponent*>(CameraComponent));
	QueryParams.bTraceComplex = true; //충돌 감지시에 콜라이더에 대한 복잡도 설정 여부 성능 이슈 있음

	FVector MuzzleLocation = WeaponSkeletalMeshComponent->GetSocketLocation(FName(TEXT("MuzzleSocket")));
	bool bIsCollide = GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, CameraEndLocation, ECC_GameTraceChannel2, QueryParams);

	if(bIsCollide)
	{
		ASCharacter* HitCharacter = Cast<ASCharacter>(HitResult.GetActor());
		if(IsValid(HitCharacter))
		{
			FDamageEvent DamageEvent;

			FString BoneNameString = HitResult.BoneName.ToString();
			UKismetSystemLibrary::PrintString(this, BoneNameString);
			DrawDebugSphere(GetWorld(), HitResult.Location, 3.f, 16, FColor::Red, true, 20.f, 0U, 5.f);

			float Damage = BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase) ? 100.f : 10.f;
			ApplyDamageAndDrawLine_Server(MuzzleLocation, HitResult.Location, HitCharacter, Damage, DamageEvent, GetController(), this);
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!IsValid(AnimInstance))
	{
		return;
	}

	if(!AnimInstance->Montage_IsPlaying(RifleFireAnimMontage))
	{
		AnimInstance->Montage_Play(RifleFireAnimMontage);
		PlayAttackMontage_Server();
	}

	if(IsValid(FireShake))
	{
		//컨트롤러가 내거일 때만 카메라 쉐이킹
		if(GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->ClientStartCameraShake(FireShake);
		}
	}
}

void ASTPSCharacter::StartIronSight(const FInputActionValue& InValue)
{
	UKismetSystemLibrary::PrintString(this, TEXT("StartIronSight"));
	TargetFOV = 30.f;
}

void ASTPSCharacter::EndIronSight(const FInputActionValue& InValue)
{
	UKismetSystemLibrary::PrintString(this, TEXT("EndIronSight"));
	TargetFOV = 70.f;
}

void ASTPSCharacter::ToggleTrigger(const FInputActionValue& InValue)
{
	UKismetSystemLibrary::PrintString(this, TEXT("ToggleTrigger"));
	bIsTriggerToggle = !bIsTriggerToggle;
}

void ASTPSCharacter::StartFire(const FInputActionValue& InValue)
{
	UKismetSystemLibrary::PrintString(this, TEXT("StartFire"));
	if(bIsTriggerToggle)
	{
		GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::Fire, TimeBetweenFire, true);
	}
}

void ASTPSCharacter::StopFire(const FInputActionValue& InValue)
{
	UKismetSystemLibrary::PrintString(this, TEXT("StopFire"));
	GetWorldTimerManager().ClearTimer(BetweenShotsTimer);
}

void ASTPSCharacter::OnTakeHitRagdollRestoreTimerElapsed()
{
	TargetRagdollBlendWeight = 0.f;
	CurrentRagdollBlendWeight = 1.f;
	bIsNowRagdollBlending = true;
}

void ASTPSCharacter::SpawnLandMine()
{
	SpawnLandMine_Server();
}

void ASTPSCharacter::PlayRagdoll_NetMulticast_Implementation()
{
	if(!IsValid(GetStatComponent()))
	{
		return;
	}

	if(GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		FName PivotBoneName = FName(TEXT("spine_01"));
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PivotBoneName, true);
		TargetRagdollBlendWeight = 1.f;
		TakeHitRagdollRestoreTimerDelegate.BindUObject(this, &ThisClass::OnTakeHitRagdollRestoreTimerElapsed);
		GetWorld()->GetTimerManager().SetTimer(TakeHitRagdollRestoreTimer, TakeHitRagdollRestoreTimerDelegate, 1.f, false);
	}
}

void ASTPSCharacter::DrawLine_NetMulticast_Implementation(const FVector& InDrawStart, const FVector& InDrawEnd)
{
	DrawDebugLine(GetWorld(), InDrawStart, InDrawEnd, FColor(255,255,255,64), false, 0.1f, 0, .5f);
}

void ASTPSCharacter::ApplyDamageAndDrawLine_Server_Implementation(const FVector& InDrawStart, const FVector& InDrawEnd,
                                                                  ACharacter* InHittedCharacter, float InDamage, FDamageEvent const& InDamageEvent, AController* InEventInstigator,
                                                                  AActor* InDamageCauser)
{
	if(IsValid(InHittedCharacter))
	{
		InHittedCharacter->TakeDamage(InDamage, InDamageEvent, InEventInstigator, InDamageCauser);
	}

	DrawLine_NetMulticast(InDrawStart, InDrawEnd);
}

void ASTPSCharacter::PlayAttackMontage_NetMulticast_Implementation()
{
	if(!HasAuthority() && GetOwner() != UGameplayStatics::GetPlayerController(this, 0))
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(!IsValid(AnimInstance))
		{
			return;
		}
		if(!AnimInstance->Montage_IsPlaying(RifleFireAnimMontage))
		{
			AnimInstance->Montage_Play(RifleFireAnimMontage);
		}
	}
}

void ASTPSCharacter::PlayAttackMontage_Server_Implementation()
{
	PlayAttackMontage_NetMulticast();
}


void ASTPSCharacter::SpawnLandMine_Server_Implementation()
{
	if(IsValid(LandMineClass))
	{
		FVector SpawnedLocation = (GetActorLocation() + GetActorForwardVector() * 200.f) - FVector(0.f, 0.f, 90.f);
		ASLandMine* SpawnedLandMine = GetWorld()->SpawnActor<ASLandMine>(LandMineClass, SpawnedLocation, FRotator::ZeroRotator);
		SpawnedLandMine->SetOwner(GetController());
	}
}

bool ASTPSCharacter::SpawnLandMine_Server_Validate()
{
	return true;
}

void ASTPSCharacter::UpdateAimValue_Server_Implementation(const float& InPitch, const float& InYaw)
{
	CurrentAimPitch = InPitch;
	CurrentAimYaw = InYaw;
}

void ASTPSCharacter::UpdateInputValue_Server_Implementation(const float& InForwardInputValue, const float& InRightInputValue)
{
	ForwardInputValue = InForwardInputValue;
	RightInputValue = InRightInputValue;
}



