// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void BeginDestroy() override;

	TStrongObjectPtr<class USoundWave> GetBackgroundMusic();

private:
	//배경음악 로드 경로는 "Sound/BGM" 폴더
	void LoadBackgroundMusicFromPath();

	TArray<TStrongObjectPtr<class USoundWave>> BackgroundMusics;
	
};
