// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "SwordWeapon.generated.h"

UCLASS()
class STUDYPROJECT_API ASwordWeapon : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASwordWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName);
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
