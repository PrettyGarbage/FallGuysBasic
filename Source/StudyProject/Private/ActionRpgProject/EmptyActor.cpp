// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/EmptyActor.h"

#include "Utils/DebugMacros.h"


// Sets default values
AEmptyActor::AEmptyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEmptyActor::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();

	SetActorLocation(FVector(0.f, 0.f, 50.f));
}

// Called every frame
void AEmptyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

