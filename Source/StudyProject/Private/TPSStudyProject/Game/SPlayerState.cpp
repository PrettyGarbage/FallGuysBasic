// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Game/SPlayerState.h"
#include "TPSStudyProject/Game/SGameInstance.h"
#include "TPSStudyProject/Game/SPlayerSaveGame.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

FString ASPlayerState::SaveSlotName = TEXT("PlayerCharacter0");

ASPlayerState::ASPlayerState()
{
}

void ASPlayerState::InitPlayerState()
{
	SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if(IsValid(SGameInstance))
	{
		if(SGameInstance->GetCharacterStatDataTable() != nullptr && SGameInstance->GetCharacterStatDataTable()->GetRowMap().Num() > 0)
		{
			MaxLevel = SGameInstance->GetCharacterStatDataTable()->GetRowMap().Num();
			CurrentLevel = 1;
			CurrentEXP = 0;
			MaxEXP = SGameInstance->GetCharacterStatDataTableRow(CurrentLevel)->MaxEXP;
		}
	}

	AGameModeBase* SGameMode = UGameplayStatics::GetGameMode(this);
	if(IsValid(SGameMode))
	{
		FString SavedTypeString = UGameplayStatics::ParseOption(SGameMode->OptionsString, TEXT("Saved"));
		if(SavedTypeString.Equals("true"))
		{
			USPlayerSaveGame* PlayerStateSave = Cast<USPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)); 
			if(IsValid(PlayerStateSave))
			{
				PlayerStateSave = GetMutableDefault<USPlayerSaveGame>();
			}

			SetPlayerName(PlayerStateSave->PlayerCharacterName);
			SetCurrentLevel(PlayerStateSave->CurrentLevel);
			SetCurrentEXP(PlayerStateSave->CurrentEXP);
		}
	}
	else
	{
		SetPlayerName(TEXT("DefaultPlayerName"));
		SetCurrentLevel(1);
		SetCurrentEXP(0.f);
	}

	USPlayerSaveGame* PlayerStateSave = Cast<USPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if(!IsValid(PlayerStateSave))
	{
		PlayerStateSave = GetMutableDefault<USPlayerSaveGame>();
	}

	SetPlayerName(PlayerStateSave->PlayerCharacterName);
	SetCurrentLevel(PlayerStateSave->CurrentLevel);
	SetCurrentEXP(PlayerStateSave->CurrentEXP);
	SetCurrentTeamType(PlayerStateSave->TeamType);

	SavePlayerState();
}

void ASPlayerState::SetCurrentLevel(int32 InCurrentLevel)
{
	int32 PrevCurrentLevel = CurrentLevel;
	int32 Level = FMath::Clamp<int32>(InCurrentLevel, 1, MaxLevel);

	FSStatTableRow* StatTableRow = SGameInstance->GetCharacterStatDataTableRow(CurrentLevel);
	if(StatTableRow != nullptr)
	{
		CurrentLevel = Level;
		MaxEXP = StatTableRow->MaxEXP;
		OnCurrentLevelChangedDelegate.Broadcast(PrevCurrentLevel, CurrentLevel);
	}

	SavePlayerState();
}

void ASPlayerState::SetCurrentEXP(float InCurrentEXP)
{
	float PrevEXP = CurrentEXP;
	CurrentEXP = FMath::Clamp<float>(InCurrentEXP, 0, MaxEXP);

	if(MaxEXP - KINDA_SMALL_NUMBER < CurrentEXP)
	{
		SetCurrentLevel(GetCurrentLevel() + 1);
		CurrentEXP = 0;
	}

	OnCurrentEXPChangedDelegate.Broadcast(PrevEXP, CurrentEXP);
}

void ASPlayerState::SavePlayerState()
{
	USPlayerSaveGame* PlayerStateSave = NewObject<USPlayerSaveGame>();
	PlayerStateSave->PlayerCharacterName = GetPlayerName();
	PlayerStateSave->CurrentLevel = GetCurrentLevel();
	PlayerStateSave->CurrentEXP = GetCurrentEXP();

	if (true == UGameplayStatics::SaveGameToSlot(PlayerStateSave, SaveSlotName, 0))
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Saved.")));
	}
}
