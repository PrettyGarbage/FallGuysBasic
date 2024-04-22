// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UUserHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar();

	void AddHealthBar();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UWrapBox> HealthBarWrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UHeart> HeartWidget;

	TArray<UHeart*> HeartArray;
};
