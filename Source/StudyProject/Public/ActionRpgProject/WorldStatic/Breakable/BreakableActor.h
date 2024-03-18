// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Interfaces/HitInterface.h"
#include "GameFramework/Actor.h"
#include "BreakableActor.generated.h"

UCLASS()
class STUDYPROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UGeometryCollectionComponent> GeometryCollectionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

private:
	UPROPERTY(EditAnywhere, Category="Brakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	uint8 bBroken : 1 = false;
};
