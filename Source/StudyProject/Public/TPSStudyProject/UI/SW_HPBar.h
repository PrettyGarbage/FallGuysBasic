// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SW_Bar.h"
#include "SW_HPBar.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USW_HPBar : public USW_Bar
{
	GENERATED_BODY()

public:
	void SetMaxHP(float InMaxHP);

	void InitializeHPBarWidget(class USStatComponent* InStatComponent);

	UFUNCTION()
	void OnMaxHPChange(float InPrevMaxHP, float InNewMaxHP);

	UFUNCTION()
	void OnCurrentHPChange(float InPrevHP, float InNewHP);

protected:
	virtual void NativeConstruct() override;
};
