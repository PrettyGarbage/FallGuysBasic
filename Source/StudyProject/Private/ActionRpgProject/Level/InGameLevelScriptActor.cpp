// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Level/InGameLevelScriptActor.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AInGameLevelScriptActor::AInGameLevelScriptActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	
}

