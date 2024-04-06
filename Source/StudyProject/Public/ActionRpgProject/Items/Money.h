// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Money.generated.h"

UCLASS()
class STUDYPROJECT_API AMoney : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMoney();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Money", meta=(AllowPrivateAccess="true"))
	FDataTableRowHandle MoneyData;

};
