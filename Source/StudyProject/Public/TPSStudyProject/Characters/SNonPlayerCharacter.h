// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "SNonPlayerCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API ASNonPlayerCharacter : public ASCharacter
{
	GENERATED_BODY()

	friend class UBTTask_Attack;

public:
	// Sets default values for this character's properties
	ASNonPlayerCharacter();

	virtual void BeginPlay() override;

	bool IsNowAttacking() const { return bIsNowAttacking; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetWidget(USUserWidget* InUserWidget) override;
	
private:
	void Attack();

	UFUNCTION()
	void OnAttackAnimMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);

private:
	float AttackRange = 200.f;
	
	float AttackRadius = 50.f;

	bool bIsNowAttacking = false;

	float EXP = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ASNonPlayerCharacter", Meta=(AllowPrivateAccess))
	TObjectPtr<class USWidgetComponent> WidgetComponent;
};
