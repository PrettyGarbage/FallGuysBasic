// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionAICharacter.h"
#include "ActionBossAICharacter.generated.h"

UCLASS()
class STUDYPROJECT_API AActionBossAICharacter : public AActionAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionBossAICharacter();

	void CheckHit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitializePawnSensingComponent();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float DetectRange = 3000.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	float AttackDamage = 20.f;
};
