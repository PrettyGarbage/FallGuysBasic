// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

UCLASS()
class STUDYPROJECT_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

	void OnControllerDead(class ASPlayerController* InDeadController);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASGameMode)
	int32 EndingTime = 15;

	int32 RemainWaitingTimerForEnding = 15;

private:
	UFUNCTION()
	void OnMainTimerElapsed();

	void NotifyToAllPlayer(const FString& NotificationString);

public:
	FTimerHandle MainTimerHandle;

	UPROPERTY()
	TObjectPtr<class ASGameState> SGameState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=ASGameMode)
	int32 WaitingTime = 15;

	int32 RemainWaitingTimerForPlaying = 15;

	int32 MinimumPlayerCountForPlaying = 2;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASGameMode, Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<class ASPlayerController>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASGameMode, Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<class ASPlayerController>> DeadPlayerControllers;
	
};
