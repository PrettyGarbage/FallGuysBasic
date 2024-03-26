// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Components/AttributeComponent.h"


// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttributeComponent::ReceiveDamage(float InDamage)
{
	Health = FMath::Clamp(Health - InDamage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::GetStaminaValue() const
{
	return Stamina;
}

float UAttributeComponent::GetEXPPercent() const
{
	return EXP / MaxEXP;
}

float UAttributeComponent::GetHealthValue() const
{
	return Health;
}

bool UAttributeComponent::IsAlive()
{
	return Health > KINDA_SMALL_NUMBER;
}

int32 UAttributeComponent::LevelUp()
{
	EXP = 0;
	MaxEXP *= 1.5f;
	
	return ++Level;
}

void UAttributeComponent::UseStamina(float InStamina)
{
	Stamina = FMath::Clamp(Stamina - InStamina, 0.f, MaxStamina);
}

void UAttributeComponent::RestoreStamina(float InDeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRestoreRate * InDeltaTime, 0.f, MaxStamina);
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

