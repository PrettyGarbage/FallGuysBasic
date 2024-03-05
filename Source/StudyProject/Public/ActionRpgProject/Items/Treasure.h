// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Treasure.generated.h"

UCLASS()
class STUDYPROJECT_API ATreasure : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATreasure();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> PickSound;

	UPROPERTY(EditAnywhere, Category= "Treasure Properties")
	int32 GoldMount;
};
