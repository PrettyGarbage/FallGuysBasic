// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Game/ActionGameMode.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Game/ActionPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"

AActionGameMode::AActionGameMode()
{
	DefaultPawnClass = AActionCharacter::StaticClass();
}

void AActionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AActionPlayerState* ActionPlayerState = NewPlayer->GetPlayerState<AActionPlayerState>();
	if(IsValid(ActionPlayerState))
	{
		ActionPlayerState->LoadPlayerState();
	}
}

void AActionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	AActionPlayerState* ActionPlayerState = Exiting->GetPlayerState<AActionPlayerState>();
	if(IsValid(ActionPlayerState))
	{
		ActionPlayerState->SavePlayerState();
	}
	
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("LogOut"));

}
