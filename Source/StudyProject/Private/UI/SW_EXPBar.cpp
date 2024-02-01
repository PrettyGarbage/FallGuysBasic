// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_EXPBar.h"

#include "Components/ProgressBar.h"
#include "Game/SPlayerState.h"

void USW_EXPBar::SetMaxEXP(float InMaxEXP)
{
	SetMaxFigure(InMaxEXP);
}

void USW_EXPBar::OnCurrentEXPChange(float InPrevEXP, float InNewEXP)
{
	if(IsValid(Bar))
	{
		const float Percentage = MaxFigure > KINDA_SMALL_NUMBER ? InNewEXP / MaxFigure : 0.f;

		Bar->SetPercent(Percentage);
	}
}

void USW_EXPBar::InitializeEXPBarWidget(ASPlayerState* NewPlayerState)
{
	OnCurrentEXPChange(0, NewPlayerState->GetCurrentLevel());
}
