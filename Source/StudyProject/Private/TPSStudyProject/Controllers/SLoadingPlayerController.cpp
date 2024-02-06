// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Controllers/SLoadingPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void ASLoadingPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	AGameModeBase* SGameMode = UGameplayStatics::GetGameMode(this);
	if(SGameMode)
	{
		FString NextLevelString = UGameplayStatics::ParseOption(SGameMode->OptionsString, TEXT("NextLevel"));
		FString SavedTypeString = UGameplayStatics::ParseOption(SGameMode->OptionsString, TEXT("Saved"));
		UGameplayStatics::OpenLevel(SGameMode, *NextLevelString, false, *SavedTypeString);
	}
}
