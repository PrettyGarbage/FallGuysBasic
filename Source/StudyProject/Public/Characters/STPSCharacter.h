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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	UFUNCTION(Server, Unreliable)
	void UpdateInputValue_Server(const float& InForwardInputValue, const float& InRightInputValue);

	UFUNCTION(Server, Unreliable)
	void UpdateAimValue_Server(const float& InPitch, const float& InYaw);

	UFUNCTION(Server, Unreliable)
	void PlayAttackMontage_Server();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayAttackMontage_NetMulticast();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TObjectPtr<class USInputConfigData> PlayerCharacterInputConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> PlayerCharacterInputMappingContext;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = ASTPSCharacter, meta = (AllowPrivateAccess))
	float ForwardInputValue;

	float PreviousForwardInputValue = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = ASTPSCharacter, meta = (AllowPrivateAccess))
	float RightInputValue;

	float PreviousRightInputValue = 0.f;

	float TargetFOV = 70.f;

	float CurrentFOV = 70.f;

	bool bIsTriggerToggle = false;

	UPROPERTY(EditDefaultsOnly, Category=ASPlayerCharacter, meta = (AllowPrivateAccess))
	float FirePerMinute = 600;

	FTimerHandle BetweenShotsTimer;

	float TimeBetweenFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= ASPlayerCharacter, meta=(AllowPrivateAccess))
	TObjectPtr<class UAnimMontage> RifleFireAnimMontage;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = ASTPSCharacter, meta = (AllowPrivateAccess))
	float CurrentAimPitch = 0.f;

	float PreviousAimPitch = 0.f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = ASTPSCharacter, meta = (AllowPrivateAccess))
	float CurrentAimYaw = 0.f;

	float PreviousAimYaw = 0.f;

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
