// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SFlyable.h"
#include "UObject/Object.h"
#include "SPigeon.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USPigeon : public UObject, public ISFlyable
{
	GENERATED_BODY()

public:
	USPigeon();

	virtual void Fly() override;

	const FString& GetName() const { return Name; }

	void SetName(const FString& InName) {Name = InName;}

	int32 GetID() const {return ID;}

	void SetID(const int32 InID) {ID = InID;}

	virtual void Serialize(FArchive& Ar) override;

private:
	UPROPERTY()
	FString Name;
	UPROPERTY()
	int32 ID;
};
