// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "ActionRpgProject/Interfaces/PickUpInterface.h"
#include "ActionCharacter.generated.h"



UCLASS()
class STUDYPROJECT_API AActionCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItemBase* InItem) override;
	virtual void AddGold(ATreasure* InTreasure) override;

	UFUNCTION(BlueprintCallable)
	void AddGold(int32 InGold);

	virtual int32 PlayAttackMontage() override;

	//UFUNCTION(BlueprintCallable)
	
	FORCEINLINE ECharacterState GetCharacterState() const { return CurrentState; }

	//Weapon
	void EquipWeapon(ASwordWeapon* OverlappingSword);
	
	void AttachWeaponToBack();
	void AttachWeaponToHand();
	void FinishEquipping();

	/* Combo */
	bool CheckCanNextCombo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	/* Enhanced Input */ 
	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void Equip(const FInputActionValue& InValue);

	virtual void Attack(const FInputActionValue& InValue) override;

	virtual void Jump() override;

	void Inventory(const FInputActionValue& InValue);

	void Dodge();

	void PlayEquipMontage(FName SectionName);

	virtual bool CanAttack() override;

	bool CanDisarm() const;

	bool CanArm() const;

	void Arm();

	void Disarm();

	virtual void Die() override;

	/* UI */
	void InitializeOverlay();
	
	void SetHUDHealth();

	void SetHealthBarValue();

	/* Combo */
	UFUNCTION()
	void FinishAttack(UAnimMontage* InAnimMontage, bool bInterrupted);
	
private:
	ECharacterState CurrentState = ECharacterState::ECS_UnEquipped;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputConfigDatas> InputConfigData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Inventory, meta=(AllowPrivateAccess=true))
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<class AItemBase> OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> EquipMontage;

	UPROPERTY()
	TObjectPtr<class UUIOverlay> UIOverlay;

	UPROPERTY()
	TObjectPtr<class UUIInventory> UIInventory;

	/* Combo */
	UPROPERTY(VisibleAnywhere)
	int32 AttackComboCount = 0;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsPressedAttack : 1;
};
