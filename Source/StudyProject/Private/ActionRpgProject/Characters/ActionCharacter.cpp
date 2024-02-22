// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/ActionCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionRpgProject/Inputs/InputConfigDatas.h"
#include "ActionRpgProject/Items/SwordWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
AActionCharacter::AActionCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(InputConfigData->MoveAction, ETriggerEvent::Triggered, this, &AActionCharacter::Move);
		EnhancedInputComponent->BindAction(InputConfigData->LookAction, ETriggerEvent::Triggered, this, &AActionCharacter::Look);
		EnhancedInputComponent->BindAction(InputConfigData->JumpAction, ETriggerEvent::Started, this, &AActionCharacter::Jump);
		EnhancedInputComponent->BindAction(InputConfigData->EquipAction, ETriggerEvent::Triggered, this, &AActionCharacter::Equip);
		EnhancedInputComponent->BindAction(InputConfigData->AttackAction, ETriggerEvent::Triggered, this, &AActionCharacter::Attack);
	}
}

void AActionCharacter::Move(const FInputActionValue& InValue)
{
	if(CurrentActionState == EActionState::EAS_Attacking) return;
	
	FVector2d MovementVector = InValue.Get<FVector2d>();

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AActionCharacter::Look(const FInputActionValue& InValue)
{
	FVector2d LookAxisVector = InValue.Get<FVector2d>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AActionCharacter::Equip(const FInputActionValue& InValue)
{
	ASwordWeapon* OverlappingSword = Cast<ASwordWeapon>(OverlappingItem);
	if(IsValid(OverlappingSword))
	{
		OverlappingSword->Equip(GetMesh(), FName("RightHandSocket"));
		CurrentState = ECharacterState::ECS_Equipped;
	}
}

void AActionCharacter::Attack(const FInputActionValue& InValue)
{
	if(CanAttack())
	{
		PlayAttackMontage();
		CurrentActionState = EActionState::EAS_Attacking;
	}
}

void AActionCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) && IsValid(AttackMontage))
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 Selection = FMath::RandRange(0, 3);
		const FString BaseString = "Attack";
		FName SectionName = FName(BaseString + FString::FromInt(Selection));

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

bool AActionCharacter::CanAttack() const
{
	return CurrentActionState == EActionState::EAS_None
	&& CurrentState != ECharacterState::ECS_UnEquipped;
}





