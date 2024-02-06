// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Components/SWidgetComponent.h"

#include "TPSStudyProject/UI/SUserWidget.h"

void USWidgetComponent::InitWidget()
{
	Super::InitWidget();

	USUserWidget* SWidget = Cast<USUserWidget>(GetWidget());
	if(IsValid(SWidget))
	{
		SWidget->SetOwningActor(GetOwner());
	}
}
