// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SHUD.h"

#include "Components/SStatComponent.h"
#include "Components/TextBlock.h"
#include "Game/SGameInstance.h"
#include "Game/SPlayerState.h"
#include "UI/SW_EXPBar.h"
#include "UI/SW_HPBar.h"

void USHUD::BindStatComponent(USStatComponent* InStatComponent)
{
	if(IsValid(InStatComponent))
	{
		StatComponent = InStatComponent;
		StatComponent->OnCurrentHPChangeDelegate.AddDynamic(HPBar, &USW_HPBar::OnCurrentHPChange);
		StatComponent->OnMaxHPChangeDelegate.AddDynamic(HPBar, &USW_HPBar::OnMaxHPChange);
	}

	USGameInstance* GameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstance))
	{
		if(GameInstance->GetCharacterStatDataTable() != nullptr && GameInstance->GetCharacterStatDataTable()->GetRowMap().Num() > 0)
		{
			float MaxHP = GameInstance->GetCharacterStatDataTableRow(1)->MaxHP;
			HPBar->SetMaxHP(MaxHP);
			HPBar->InitializeHPBarWidget(StatComponent.Get());
		}
	}
}

void USHUD::BindPlayerState(ASPlayerState* InPlayerState)
{
	if(IsValid(InPlayerState))
	{
		PlayerState = InPlayerState;
		PlayerState->OnCurrentEXPChangedDelegate.AddDynamic(EXPBar, &USW_EXPBar::OnCurrentEXPChange);
		PlayerState->OnCurrentLevelChangedDelegate.AddDynamic(this, &ThisClass::UpdateLevelText);

		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));
		EXPBar->SetMaxEXP(PlayerState->GetMaxEXP());
		EXPBar->InitializeEXPBarWidget(PlayerState.Get());
		UpdateLevelText(0, PlayerState->GetCurrentLevel());
	}
}

void USHUD::UpdateLevelText(int32 InOldLevel, int32 InNewLevel)
{
	FString LevelString = FString::Printf(TEXT("%d"), InNewLevel);

	LevelText->SetText(FText::FromString(LevelString));
}
