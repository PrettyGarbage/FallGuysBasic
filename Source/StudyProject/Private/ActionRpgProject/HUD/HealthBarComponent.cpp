// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/HealthBarComponent.h"

#include "ActionRpgProject/HUD/HealthBar.h"
#include "Components/ProgressBar.h"


// Sets default values for this component's properties
UHealthBarComponent::UHealthBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthBarComponent::SetHealthPercentage(float HealthPercentage)
{
	if(!IsValid(HealthBarWidget))
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if(IsValid(HealthBarWidget))
	{
		HealthBarWidget->HealthBar->SetPercent(HealthPercentage);
	}
}

