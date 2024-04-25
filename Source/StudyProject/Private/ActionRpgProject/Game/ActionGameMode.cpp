// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Game/ActionGameMode.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

AActionGameMode::AActionGameMode()
{
	DefaultPawnClass = AActionCharacter::StaticClass();
}

void AActionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("PostLogin"));
}

void AActionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Logout"));
}
