// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Characters/BaseCharacter.h"
#include "ActionAICharacter.generated.h"

UCLASS()
class STUDYPROJECT_API AActionAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionAICharacter();

	virtual void BeginPlay() override;

	bool IsNowAttacking() const { return bIsAttacking; }

	bool IsHitReact() const { return bHitReact; }

	void SetHitReactEndState();

	void CallAttackLogic();

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void InitializeAnimMontageEvent();

	void Attack() override;

	void HandleDamage(float DamageAmount) override;

	UFUNCTION()
	void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

	void ShowHPbar(bool bShow);

	
private:
	UPROPERTY(EditAnywhere, Category="AI", Meta=(AllowPrivateAccess))
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, Category="AI", Meta=(AllowPrivateAccess))
	float AttackRadius = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", Meta=(AllowPrivateAccess))
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", Meta=(AllowPrivateAccess))
	bool bHitReact = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Combat", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::EES_None;
	
	//components
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	TObjectPtr<class UHealthBarComponent> HealthBarWidget;
	
};
