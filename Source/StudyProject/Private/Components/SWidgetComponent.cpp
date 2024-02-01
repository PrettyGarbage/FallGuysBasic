// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SWidgetComponent.h"

#include "UI/SUserWidget.h"

void USWidgetComponent::InitWidget()
{
	Super::InitWidget();

	USUserWidget* SWidget = Cast<USUserWidget>(GetWidget());
	if(IsValid(SWidget))
	{
		SWidget->SetOwningActor(GetOwner());
	}
}
