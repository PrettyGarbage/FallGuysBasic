// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SUserWidget.h"
#include "SW_Bar.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USW_Bar : public USUserWidget
{
	GENERATED_BODY()

public:
	USW_Bar(const FObjectInitializer& ObjectInitializer);

	void SetMaxFigure(float InMaxFigure);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="USW_Bar")
	TObjectPtr<class UProgressBar> Bar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="USW_Bar")
	float MaxFigure;
};
