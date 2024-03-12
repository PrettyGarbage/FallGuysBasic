// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "ActionRpgProject/Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type InType);
	
protected:
	virtual void BeginPlay() override;

	virtual void Attack(const FInputActionValue& InValue);

	virtual void Die();

	/* Attack Montage */
	
	virtual void PlayAttackMontage();

	virtual void PlayHitReactMontage(const FName& SectionName);

	virtual void DirectionHitReact(const FVector& ImpactPoint);
	
	virtual bool CanAttack();
	
	UPROPERTY(VisibleAnywhere, Category= "Weapon")
	TObjectPtr<class ASwordWeapon> EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category="Montages")
	TObjectPtr<class UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Montages")
	TObjectPtr<class UAnimMontage> DeathMontage;

	/* Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAttributeComponent> AttributeComponent;

	/* Sounds and Visual Effects */
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<class USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Visual Effects")
	TObjectPtr<class UParticleSystem> HitParticle;
};
