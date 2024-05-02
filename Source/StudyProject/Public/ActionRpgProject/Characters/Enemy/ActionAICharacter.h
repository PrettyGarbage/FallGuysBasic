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

	void InitEnemyStatData();

	bool IsNowAttacking() const { return bIsAttacking; }

	bool IsHitReact() const { return bHitReact; }

	void SetHitReactEndState();

	void CallAttackLogic();

	EEnemyState GetEnemyState() const { return EnemyState; }

	void ShowPivot(bool bShow);

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void InitializeSubsystemData();
	
	void InitializeAnimMontageEvent();

	void Attack() override;

	void HandleDamage(float DamageAmount) override;

	UFUNCTION()
	void OnAttackMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

	void ShowHPbar(bool bShow);

	void SpawnDefaultWeapon();

	void RemoveFromActorManagerSubsystem();

protected:
	UPROPERTY(EditAnywhere, Category="Common", Meta=(AllowPrivateAccess))
	FString EnemyName = "Enemy";
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Combat", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::EES_None;
	
	UPROPERTY(EditAnywhere, Category="AI", Meta=(AllowPrivateAccess))
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, Category="AI", Meta=(AllowPrivateAccess))
	float AttackRadius = 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", Meta=(AllowPrivateAccess))
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI", Meta=(AllowPrivateAccess))
	bool bHitReact = false;
	
	//components
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	TObjectPtr<class UHealthBarComponent> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	TObjectPtr<class UWidgetComponent> IconWidget;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASwordWeapon> WeaponClass;
	
};
