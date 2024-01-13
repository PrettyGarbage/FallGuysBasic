// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SPlayerController.h"

ASPlayerController::ASPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis(TEXT("UpDown"), this, &ThisClass::UpDown);
	//InputComponent->BindAxis(TEXT("LeftRight"), this, &ThisClass::LeftRight);
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}

void ASPlayerController::LeftRight(float InAxisValue)
{
	//UE_LOG(LogTemp, Log, TEXT("LeftRight: %f"), InAxisValue);
}

void ASPlayerController::UpDown(float InAxisValue)
{
	//UE_LOG(LogTemp, Log, TEXT("UpDown: %f"), InAxisValue);
}
