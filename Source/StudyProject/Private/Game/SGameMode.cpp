// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"

#include "Characters/SPlayerPawn.h"
#include "Controllers/SPlayerController.h"

ASGameMode::ASGameMode()
{
	DefaultPawnClass = ASPlayerPawn::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	
}
