// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Npcs/Bird.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionRpgProject/Inputs/InputConfigDatas.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABird::ABird()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleRadius(15.f);
	CapsuleComponent->SetCapsuleHalfHeight(20.f);
	SetRootComponent(CapsuleComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//RPG Character에서는 스프링암의 회전을 풀고 캐릭터의 회전은 막았지만 Bird에서는 그 반대로 설정
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(InputConfigData->MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(InputConfigData->LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
}

void ABird::Move(const FInputActionValue& InValue)
{
	FVector2d MovementVector = InValue.Get<FVector2d>();

	ForwardInputValue = MovementVector.X;
	RightInputValue = MovementVector.Y;

	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void ABird::Look(const FInputActionValue& InValue)
{
	FVector2d LookAxisVector = InValue.Get<FVector2d>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ABird::Turn(const FInputActionValue& InValue)
{
}

