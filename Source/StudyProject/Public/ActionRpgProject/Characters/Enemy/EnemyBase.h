// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Characters/CharacterTypes.h"
#include "ActionRpgProject/Interfaces/HitInterface.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class STUDYPROJECT_API AEnemyBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PlayHitReactMontage(const FName& SectionName);

	void Die();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionHitReact(const FVector& ImpactPoint);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	EDeathPose GetDeathPoseEnumValue(FName InName);

	TMap<FName, EDeathPose> DeathPoseMap = {
		{FName(TEXT("Death1")), EDeathPose::EDP_Death1},
		{FName(TEXT("Death2")), EDeathPose::EDP_Death2},
		{FName(TEXT("Death3")), EDeathPose::EDP_Death3},
		{FName(TEXT("Death4")), EDeathPose::EDP_Death4}
	};
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UHealthBarComponent> HealthBarWidget;
	
	//Montages
	UPROPERTY(EditAnywhere, Category="Montages")
	TObjectPtr<class UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Montages")
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<class USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Visual Effects")
	TObjectPtr<class UParticleSystem> HitParticle;
};
