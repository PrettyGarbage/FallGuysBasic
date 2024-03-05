// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/WorldStatic/Breakable/BreakableActor.h"

#include "ActionRpgProject/Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"


// Sets default values
ABreakableActor::ABreakableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetGenerateOverlapEvents(true);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if(bBroken) return;
	
	bBroken = true;

	UWorld* World = GetWorld();
	if(IsValid(World) && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 100.f;

		int32 SelectionNum = FMath::RandRange(0, TreasureClasses.Num() - 1);
		
		GetWorld()->SpawnActor<ATreasure>(TreasureClasses[SelectionNum], Location, GetActorRotation());
	}
}










