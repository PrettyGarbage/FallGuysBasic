// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SPlayerController.h"

#include "Characters/SCharacter.h"
#include "Components/SStatComponent.h"
#include "Game/SPlayerState.h"
#include "UI/SHUD.h"
#include "Blueprint/UserWidget.h"

ASPlayerController::ASPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASPlayerController::ToggleMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Log Message"));
	if(!bIsMenuOn)
	{
		MenuUIInstance->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(MenuUIInstance->GetCachedWidget());
		SetInputMode(Mode);

		bShowMouseCursor = true;
	}
	else
	{
		MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);

		bShowMouseCursor = false;
	}

	bIsMenuOn = !bIsMenuOn;
	SetPause(bIsMenuOn);
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if(IsValid(HUDWidgetClass))
	{
		HUDWidget = CreateWidget<USHUD>(this, HUDWidgetClass);
		if(IsValid(HUDWidget))
		{
			HUDWidget->AddToViewport();

			ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
			if(IsValid(SPlayerState))
			{
				HUDWidget->BindPlayerState(SPlayerState);
			}

			ASCharacter* SCharacter = GetPawn<ASCharacter>();
			if(IsValid(SCharacter))
			{
				USStatComponent* StatComponent = SCharacter->GetStatComponent();
				if(IsValid(StatComponent))
				{
					HUDWidget->BindStatComponent(StatComponent);
				}
			}
		}
	}

	if(IsValid(MenuUIClass))
	{
		MenuUIInstance = CreateWidget<UUserWidget>(this, MenuUIClass);
		if(IsValid(MenuUIInstance))
		{
			MenuUIInstance->AddToViewport(3);

			MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(IsValid(CrossHairUIClass))
	{
		UUserWidget* CrossHairUI = CreateWidget<UUserWidget>(this, CrossHairUIClass);
		if(IsValid(CrossHairUI))
		{
			CrossHairUI->AddToViewport(1);
			CrossHairUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
