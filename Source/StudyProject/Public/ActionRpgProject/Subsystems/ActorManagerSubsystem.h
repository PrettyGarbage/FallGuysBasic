// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API UActorManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void DrawNearIconAroundPlayer();

	void AddEnemy(class AActionAICharacter* InEnemy);

	void RemoveEnemy(class AActionAICharacter* InEnemy);

	void ClearEnemies();

private:
	//레벨에 로드된 모든 적을 저장하는 배열
	TArray<TWeakObjectPtr<class AActionAICharacter>> WorldEnemies;

	//반경안에 있는 적을 저장하는 배열
	TArray<TWeakObjectPtr<AActionAICharacter>> EnemiesInRange;

	UPROPERTY(EditAnywhere, Category="UI", Meta=(AllowPrivateAccess))
	float SearchDistance = 500.f;
	
};
