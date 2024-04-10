// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/UserHealthBar.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/HUD/Heart.h"
#include "Components/WrapBox.h"

void UUserHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateHealthBar();
}

//개선점이 있을거라고 생각함. 클리어하고 다시 크리에이트하는게 맘에 안듦
void UUserHealthBar::UpdateHealthBar() const
{
	HealthBarWrapBox->ClearChildren();

	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(GetOwningPlayerPawn());
	if(IsValid(ActionCharacter))
	{
		UAttributeComponent* AttributeComponent = ActionCharacter->GetComponentByClass<UAttributeComponent>();

		if(IsValid(AttributeComponent))
		{
			float CurrentHealth = AttributeComponent->GetHealthValue();
			for(int32 i = 0; i < AttributeComponent->GetMaxHealthValue(); i++)
			{
				UHeart* Heart = CreateWidget<UHeart>(GetWorld(), HeartWidget);
				
				if(IsValid(Heart))
				{
					const bool bIsHalfHeart = i + 0.5f == CurrentHealth;
					const bool bIsFullHeart = i < CurrentHealth;
					Heart->SetHealth(bIsHalfHeart ? 0.5 : bIsFullHeart ? 1 : 0);
				}

				if(IsValid(HealthBarWrapBox))
				{
					HealthBarWrapBox->AddChild(Heart);
				}
			}
		}
	}
}
