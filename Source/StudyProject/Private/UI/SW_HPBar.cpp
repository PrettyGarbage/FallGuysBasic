// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SW_HPBar.h"

#include "Characters/SCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/SStatComponent.h"

void USW_HPBar::SetMaxHP(float InMaxHP)
{
	SetMaxFigure(InMaxHP);
}

void USW_HPBar::InitializeHPBarWidget(USStatComponent* InStatComponent)
{
	OnCurrentHPChange(0, InStatComponent->GetCurrentHP());
}

void USW_HPBar::OnMaxHPChange(float InPrevMaxHP, float InNewMaxHP)
{
	SetMaxFigure(InNewMaxHP);

	OnCurrentHPChange(0, InPrevMaxHP);
}

void USW_HPBar::OnCurrentHPChange(float InPrevHP, float InNewHP)
{
	if(IsValid(Bar))
	{
		if(MaxFigure > KINDA_SMALL_NUMBER)
		{
			Bar->SetPercent(InNewHP / MaxFigure);
		}
		else
		{
			Bar->SetPercent(0.f);
		}
	}
}

void USW_HPBar::NativeConstruct()
{
	Super::NativeConstruct();

	ASCharacter* OwningCharacter = Cast<ASCharacter>(OwningActor);
	if(IsValid(OwningCharacter))
	{
		OwningCharacter->SetWidget(this);
	}
}
