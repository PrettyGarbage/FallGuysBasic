// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/UIOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUIOverlay::SetHealthPercent(float InPercent)
{
	if(IsValid(HealthProgressBar))
	{
		HealthProgressBar->SetPercent(InPercent);
	}
}

void UUIOverlay::SetStaminaPercent(float InPercent)
{
	if(IsValid(StaminaProgressBar))
	{
		StaminaProgressBar->SetPercent(InPercent);
	}
}

void UUIOverlay::SetGold(int32 InGold)
{
	if(IsValid(GoldText))
	{
		const FString String = FString::Printf(TEXT("%d"), InGold);
		const FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}
