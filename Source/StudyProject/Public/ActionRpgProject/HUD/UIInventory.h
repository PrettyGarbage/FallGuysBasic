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

public:
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void SetMoneyText(int32 Money);

private:
	void CloseInventory();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UAttributeComponent> AttributeComponent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MoneyText;
};
