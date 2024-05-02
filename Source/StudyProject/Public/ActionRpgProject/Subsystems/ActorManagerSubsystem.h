// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Structs/SlotStruct.h"
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

	TWeakObjectPtr<class AActionAICharacter> GetClosestEnemy(const FVector& InLocation);

	void ClearEnemies();

	void ClearSearchTimer();

	void LoadJsonData();

	FEnemyTableRow GetEnemyData(const FString& InName);

public:
	//JSON 데이터가 넘어온다고 가정했을 때의 더미 데이터
	const FString JsonData = "{ \"Enemies\" : [ { \"Name\" : \"Paladin\", \"HP\" : 100 }, { \"Name\" : \"Kraken\", \"HP\" : 1000 } ] }" ; 

	TSharedPtr<FJsonObject> EnemyJsonObject;
	TSharedRef<TJsonReader<>> EnemyJsonReader = TJsonReaderFactory<>::Create(JsonData);
	
private:
	//레벨에 로드된 모든 적을 저장하는 배열
	TArray<TWeakObjectPtr<class AActionAICharacter>> WorldEnemies;

	//반경안에 있는 적을 저장하는 배열
	TArray<TWeakObjectPtr<AActionAICharacter>> EnemiesInRange;

	UPROPERTY(EditAnywhere, Category="UI", Meta=(AllowPrivateAccess))
	float SearchDistance = 500.f;

	UPROPERTY()
	FTimerHandle SearchHandle;

	TArray<TSharedRef<FEnemyTableRow>> EnemyTableData;
};
