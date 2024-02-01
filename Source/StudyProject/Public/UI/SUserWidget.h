// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	AActor* GetOwningActor() const {return OwningActor;}

	void SetOwningActor(AActor* InOwner) {OwningActor = InOwner;}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USUserWidget", Meta = (AllowPrivateAccess))
	TObjectPtr<AActor> OwningActor;
};
