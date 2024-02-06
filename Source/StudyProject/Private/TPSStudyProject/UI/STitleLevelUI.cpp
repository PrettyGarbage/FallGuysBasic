// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/UI/STitleLevelUI.h"

#include "Components/Button.h"
#include "TPSStudyProject/Game/SPlayerSaveGame.h"
#include "TPSStudyProject/Game/SPlayerState.h"
#include "Kismet/GameplayStatics.h"


USTitleLevelUI::USTitleLevelUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USTitleLevelUI::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnNewGameButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
	SavedGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSavedGameButtonClicked);

	//교재나 다른 참고 코드
	SavedGameButton->SetIsEnabled(false);
	const ASPlayerState* SPlayerState = GetDefault<ASPlayerState>();
	if(IsValid(SPlayerState))
	{
		USPlayerSaveGame* SPlayerStateGame = Cast<USPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SPlayerState->SaveSlotName, 0));
		if(IsValid(SPlayerStateGame))
		{
			SavedGameButton->SetIsEnabled(true);
		}
	}
	
}

void USTitleLevelUI::OnNewGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Lobby?Saved=false")));
}

void USTitleLevelUI::OnExitGameButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void USTitleLevelUI::OnSavedGameButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Example")), true, FString(TEXT("NextLevel=Lobby?Saved=true")));
}
