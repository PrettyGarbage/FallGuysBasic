// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/SUIPlayerController.h"

#include "Blueprint/UserWidget.h"

void ASUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(UIWidgetClass))
	{
		UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
		if(IsValid(UIWidgetInstance))
		{
			UIWidgetInstance->AddToViewport();

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
			SetInputMode(Mode);

			bShowMouseCursor = true;
		}
	}
}
