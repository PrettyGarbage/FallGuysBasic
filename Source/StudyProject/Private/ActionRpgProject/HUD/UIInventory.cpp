// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/UIInventory.h"

#include "Kismet/KismetSystemLibrary.h"

FReply UUIInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == EKeys::I)
	{
		RemoveFromParent();

		APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer());
		if(IsValid(PlayerController))
		{
			PlayerController->bShowMouseCursor = false;

			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
