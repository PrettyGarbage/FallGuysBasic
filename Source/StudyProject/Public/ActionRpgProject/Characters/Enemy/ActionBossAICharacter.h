// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionAICharacter.h"
#include "ActionBossAICharacter.generated.h"

UCLASS()
class STUDYPROJECT_API AActionBossAICharacter : public AActionAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionBossAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
