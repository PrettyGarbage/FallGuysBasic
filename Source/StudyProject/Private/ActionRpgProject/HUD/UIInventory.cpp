// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/UIInventory.h"

#include "ActionRpgProject/Components/AttributeComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UUIInventory::NativeConstruct()
{
	Super::NativeConstruct();

	if(IsValid(GetOwningPlayer()) && IsValid(MoneyText))
	{
		AttributeComponent = GetOwningPlayer()->GetComponentByClass<UAttributeComponent>();
		if(IsValid(AttributeComponent))
		{
			UKismetSystemLibrary::PrintString(GetWorld(), "AttributeComponent is valid", true, true, FLinearColor::Green, 5.f);
			MoneyText->SetText(FText::FromString(FString::FromInt(AttributeComponent->GetGold())));
		}
	}
}

FReply UUIInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if(InKeyEvent.GetKey() == EKeys::I)
	{
		AttributeComponent = GetOwningPlayer()->GetComponentByClass<UAttributeComponent>();
		if(IsValid(AttributeComponent))
		{
			UKismetSystemLibrary::PrintString(GetWorld(), "AttributeComponent is valid", true, true, FLinearColor::Green, 5.f);
			MoneyText->SetText(FText::FromString(FString::FromInt(AttributeComponent->GetGold())));
		}
		
		CloseInventory();
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUIInventory::SetMoneyText(int32 Money)
{
	if(IsValid(MoneyText))
	{
		MoneyText->SetText(FText::FromString(FString::FromInt(Money)));
	}
}

void UUIInventory::CloseInventory()
{
	RemoveFromParent();

	APlayerController* PlayerController = Cast<APlayerController>(GetOwningPlayer());
	if(IsValid(PlayerController))
	{
		PlayerController->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}
