// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUDYPROJECT_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void ReceiveDamage(float InDamage);
	float GetHealthPercent() const;
	float GetStaminaPercent();
	float GetEXPPercent() const;
	bool IsAlive();
	int32 LevelUp();
	void UseStamina(float InStamina);
	void RestoreStamina(float InDeltaTime);
	
	FORCEINLINE void SetLevel(int32 InLevel) { Level = InLevel; }
	FORCEINLINE void AddEXP(float InEXP) { EXP += InEXP; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE void AddGold(int32 InGold) { Gold += InGold; }

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category="Actor Attributes")
	float EXP;

	UPROPERTY(VisibleAnywhere, Category="Actor Attributes")
	float MaxEXP;

	UPROPERTY(VisibleAnywhere, Category="Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category="Actor Attributes")
	float StaminaRestoreRate;
};
