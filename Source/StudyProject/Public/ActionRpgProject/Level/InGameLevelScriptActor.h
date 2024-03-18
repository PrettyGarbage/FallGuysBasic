// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGameLevelScriptActor.generated.h"

UCLASS()
class STUDYPROJECT_API AInGameLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInGameLevelScriptActor();

	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
