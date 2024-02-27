// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ActionCharacter.generated.h"


UCLASS()
class STUDYPROJECT_API AActionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type InType);

	FORCEINLINE void SetOverlappingItem(class AItemBase* InItem) { OverlappingItem = InItem; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CurrentState; }
	FORCEINLINE void SetCharacterActionState(EActionState InState) { CurrentActionState = InState; }
	

	//Animation Notify Bind Functions
	void Disarm();
	void Arm();
	void FinishEquipping();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void Move(const FInputActionValue& InValue);

	void Look(const FInputActionValue& InValue);

	void Equip(const FInputActionValue& InValue);

	void Attack(const FInputActionValue& InValue);
	
	void PlayAttackMontage();

	void PlayEquipMontage(FName SectionName);

	bool CanAttack() const;

	bool CanDisarm() const;

	bool CanArm() const;
	
	
private:
	ECharacterState CurrentState = ECharacterState::ECS_UnEquipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EActionState CurrentActionState = EActionState::EAS_None;
	
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

	UPROPERTY(VisibleAnywhere, Category= "Weapon")
	TObjectPtr<class ASwordWeapon> EquippedWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TObjectPtr<class UAnimMontage> EquipMontage;
	
};
