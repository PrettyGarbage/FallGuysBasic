// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Characters/Enemy/ActionBossAICharacter.h"

#include "ActionRpgProject/Controller/EnemyAIController.h"


// Sets default values
AActionBossAICharacter::AActionBossAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AActionBossAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}