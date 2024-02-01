// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SPlayerCharacterSettings.generated.h"

/**
 * 
 */
UCLASS(Config=PlayerCharacterMeshPaths)
class STUDYPROJECTSETTINGS_API USPlayerCharacterSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(config)
	TArray<FSoftObjectPath> PlayerCharacterMeshPaths;
};
