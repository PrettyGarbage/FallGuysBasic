// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "ActionCharacter.generated.h"


UCLASS()
class STUDYPROJECT_API AActionCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	
	FORCEINLINE void SetOverlappingItem(class AItemBase* InItem) { OverlappingItem = InItem; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CurrentState; }

	//Weapon
	void EquipWeapon(ASwordWeapon* OverlappingSword);
	
	void AttachWeaponToBack();
	void AttachWeaponToHand();
	void FinishEquipping();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void Equip(const FInputActionValue& InValue);

	virtual void Attack(const FInputActionValue& InValue) override;

	void PlayEquipMontage(FName SectionName);

	virtual bool CanAttack() override;

	bool CanDisarm() const;

	bool CanArm() const;

	void Arm();

	void Disarm();
	
	
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

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<class AItemBase> OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> EquipMontage;
	
};
