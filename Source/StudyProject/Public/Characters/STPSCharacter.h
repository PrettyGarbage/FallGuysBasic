// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SCharacter.h"
#include "STPSCharacter.generated.h"

UCLASS()
class STUDYPROJECT_API ASTPSCharacter : public ASCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTPSCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	float GetForwardInputValue() const {return ForwardInputValue;}

	float GetRightInputValue() const {return RightInputValue;}

	float GetCurrentAimPitch() const {return CurrentAimPitch;}

	float GetCurrentAimYaw() const {return CurrentAimYaw;}

	USkeletalMeshComponent* GetWeaponSkeletalMeshComponent() const {return WeaponSkeletalMeshComponent;}

	void SetMuzzleLocation(const FVector& InLocation);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void Attack(const FInputActionValue& InValue);

	void Fire();

	void StartIronSight(const FInputActionValue& InValue);

	void EndIronSight(const FInputActionValue& InValue);

	void ToggleTrigger(const FInputActionValue& InValue);

	void StartFire(const FInputActionValue& InValue);

	void StopFire(const FInputActionValue& InValue);

	UFUNCTION()
	void OnTakeHitRagdollRestoreTimerElapsed();

	void SpawnLandMine();

	UFUNCTION(Server, Reliable, WithValidation)
	void SpawnLandMine_Server();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TObjectPtr<class USInputConfigData> PlayerCharacterInputConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> PlayerCharacterInputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	float ForwardInputValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	float RightInputValue = 0.f;

	float TargetFOV = 70.f;

	float CurrentFOV = 70.f;

	bool bIsTriggerToggle = false;

	UPROPERTY(EditDefaultsOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	float FirePerMinute = 600;

	FTimerHandle BetweenShotsTimer;

	float TimeBetweenFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= ASPlayerCharacter, meta=(AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> RifleFireAnimMontage;

	float CurrentAimPitch = 0.f;

	float CurrentAimYaw = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TSubclassOf<class UCameraShakeBase> FireShake;

	FTimerHandle TakeHitRagdollRestoreTimer;

	FTimerDelegate TakeHitRagdollRestoreTimerDelegate;

	float TargetRagdollBlendWeight = 0.f;

	float CurrentRagdollBlendWeight = 0.f;

	bool bIsNowRagdollBlending = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TSubclassOf<class AActor> LandMineClass;
};
