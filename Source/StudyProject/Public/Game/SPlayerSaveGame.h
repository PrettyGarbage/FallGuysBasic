// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SPlayerSaveGame.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	None = 0,
	Red = 1,
	Blue = 2,
	End = 3,
};
UCLASS()
class STUDYPROJECT_API USPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	USPlayerSaveGame();
	
	UPROPERTY()
	FString PlayerCharacterName;

	UPROPERTY()
	int32 CurrentLevel;

	UPROPERTY()
	float CurrentEXP;

	UPROPERTY()
	ETeamType TeamType = ETeamType::Red;
};
