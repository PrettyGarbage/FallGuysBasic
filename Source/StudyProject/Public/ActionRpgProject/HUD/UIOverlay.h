// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIOverlay.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UUIOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float InPercent);
	void SetStaminaPercent(float InPercent);
	void SetGold(int32 InGold);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldText;
};
