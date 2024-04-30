// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Structs/SlotStruct.h"
#include "GameFramework/SaveGame.h"
#include "ActionPlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UActionPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UActionPlayerSaveGame();

	UPROPERTY()
	FString PlayerCharacterName;

	UPROPERTY()
	FAllItems AllItems;
};
