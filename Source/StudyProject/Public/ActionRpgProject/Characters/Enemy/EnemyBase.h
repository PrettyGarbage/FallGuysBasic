// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionHitReact(const FVector& ImpactPoint);

	virtual void GetHit(const FVector& ImpactPoint) override;

private:

	//Montages
	UPROPERTY(EditAnywhere, Category="Montages")
	TObjectPtr<class UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<class USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Visual Effects")
	TObjectPtr<class UParticleSystem> HitParticle;
};
