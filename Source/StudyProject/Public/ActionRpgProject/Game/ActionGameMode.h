// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ActionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API AActionGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AActionGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
};
