// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentHPChangeDelegate, float, InPrevCurrentHP, float, InCurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPChangeDelegate, float, InPrevMaxHP, float, InMaxHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STUDYPROJECT_API USStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USStatComponent();

	float GetMaxHP() const {return MaxHP;}

	float GetCurrentHP() const {return CurrentHP;}

	void SetMaxHP(float InMaxHP);

	void SetCurrentHP(float InCurrentHP);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnCurrentLevelChanged(int32 InPrevCurrentLevel, int32 InNewCurrentLevel);

	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentHPChanged_NetMulticast(float InPrevCurrentHP, float InNewCurrentHP);

public:
	FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;

	FOnCurrentHPChangeDelegate OnCurrentHPChangeDelegate;

	FOnMaxHPChangeDelegate OnMaxHPChangeDelegate;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
	TObjectPtr<class USGameInstance> GameInstance;

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
	float MaxHP;

	UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
	float CurrentHP;

	
};
