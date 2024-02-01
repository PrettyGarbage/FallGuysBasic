// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SMenuUI.h"

#include "Components/Button.h"
#include "Controllers/SPlayerController.h"
#include "Kismet/GameplayStatics.h"

void USMenuUI::NativeConstruct()
{
	ResumeGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnResumeGameButtonClicked);
	ReturnTitleButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnTitleButtonClicked);
	ExitGameButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitGameButtonClicked);
}

void USMenuUI::OnResumeGameButtonClicked()
{
	ASPlayerController* SPlayerController = Cast<ASPlayerController>(GetOwningPlayer());
	if(IsValid(SPlayerController))
	{
		SPlayerController->ToggleMenu();
	}
}

void USMenuUI::OnReturnTitleButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Title")));
}

void USMenuUI::OnExitGameButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
