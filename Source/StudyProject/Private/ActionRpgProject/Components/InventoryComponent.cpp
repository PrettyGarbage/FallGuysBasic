// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Components/InventoryComponent.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/HUD/UIInventory.h"
#include "ActionRpgProject/HUD/UserHealthBar.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInventoryComponent::DecreaseHP()
{
	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(GetOwner());
	if(IsValid(ActionCharacter))
	{
		UAttributeComponent* AttributeComponent = ActionCharacter->FindComponentByClass<UAttributeComponent>();
		if(IsValid(AttributeComponent))
		{
			AttributeComponent->ReceiveDamage(.5f);
			if(IsValid(HealthBarWidget))
			{
				HealthBarWidget->UpdateHealthBar();
			}
		}
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateHealthBar();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceItemToPickUp();
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

void UInventoryComponent::CreateHealthBar()
{
	if(IsValid(HealthBarWidgetClass))
	{
		HealthBarWidget = CreateWidget<UUserHealthBar>(GetWorld(), HealthBarWidgetClass);
		if(IsValid(HealthBarWidget))
		{
			HealthBarWidget->SetPositionInViewport(FVector2d(5,5), true);
			
			HealthBarWidget->AddToViewport();
		}
	}
}

void UInventoryComponent::TraceItemToPickUp()
{
	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(GetOwner());
	if(IsValid(ActionCharacter))
	{
		FVector StartPosition = ActionCharacter->GetActorLocation() - FVector(0, 0, 60.f);
		FVector EndPosition = StartPosition + ActionCharacter->GetActorForwardVector() * 300.f;
		TArray<FHitResult> HitRetArray;

		bool bIsHit = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(),
		StartPosition,
		EndPosition, 30.f,
		FName("Item"), false, TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame, HitRetArray, true);
	}
	

}

