// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"

#include "Characters/SPlayerPawn.h"
#include "Controllers/SPlayerController.h"
#include "Game/SPlayerState.h"

ASGameMode::ASGameMode()
{
	DefaultPawnClass = ASPlayerPawn::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	
}

void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if(IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}
}
