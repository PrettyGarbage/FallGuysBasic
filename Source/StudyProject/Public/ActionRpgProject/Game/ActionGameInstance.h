// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Structs/SlotStruct.h"
#include "ActionRpgProject/Subsystems/ActorManagerSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "ActionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UActionGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

};
