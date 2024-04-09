// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Components/InventoryComponent.h"

#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/HUD/UIInventory.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInventoryComponent::ActiveInventoryUI()
{
	UWorld* World = GetWorld();
	if(IsValid(World))
	{
		APlayerController* PlayerController = Cast<APlayerController>(World->GetFirstPlayerController());

		if(IsValid(PlayerController))
		{
			OpenInventory(PlayerController);
		}
		
	}
}

void UInventoryComponent::OpenInventory(APlayerController* PlayerController)
{
	if(IsValid(InventoryWidgetClass) && IsValid(PlayerController))
	{
		InventoryWidget = CreateWidget<UUIInventory>(PlayerController, InventoryWidgetClass);
		InventoryWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);
	}

	UActorComponent* AttributeComponent = GetOwner()->GetComponentByClass(UAttributeComponent::StaticClass());
	if(IsValid(AttributeComponent))
	{
		UAttributeComponent* AttributeComp = Cast<UAttributeComponent>(AttributeComponent);
		if(IsValid(AttributeComp))
		{
			InventoryWidget->SetMoneyText(AttributeComp->GetGold());
		}
	}
}

