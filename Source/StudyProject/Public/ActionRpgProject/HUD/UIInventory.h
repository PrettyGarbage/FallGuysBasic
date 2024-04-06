// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInventory.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UUIInventory : public UUserWidget
{
	GENERATED_BODY()

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
