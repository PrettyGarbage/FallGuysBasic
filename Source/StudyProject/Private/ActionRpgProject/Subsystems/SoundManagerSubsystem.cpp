// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Subsystems/SoundManagerSubsystem.h"

#include "ActionRpgProject/Define/DefineVariables.h"
#include "Kismet/KismetMathLibrary.h"

void USoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadBackgroundMusicFromPath();
}

TStrongObjectPtr<USoundWave> USoundManagerSubsystem::GetBackgroundMusic()
{
	if(BackgroundMusics.Num() > 0)
	{
		//랜덤으로 배경음악을 재생
		int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, BackgroundMusics.Num() - 1);
		USoundWave* SoundBase = BackgroundMusics[RandomIndex].Get();

		return TStrongObjectPtr(SoundBase);
	}

	return nullptr;
}

void USoundManagerSubsystem::LoadBackgroundMusicFromPath()
{
	TArray<FString> SoundFileNames;
	FString RootFolderFullPath = FPaths::ProjectContentDir() + GBackgroundMusicPath;
	FPaths::NormalizeDirectoryName(RootFolderFullPath);
	SoundFileNames.Empty();

	FString FinalPath = RootFolderFullPath + "/*";

	IFileManager::Get().FindFiles(SoundFileNames, *FinalPath, true, false);
	
	//지정된 경로에 있는 SoundWave 파일을 로드
	for(const FString& SoundFileName : SoundFileNames)
	{
		FString SoundName = SoundFileName.Left(SoundFileName.Find(TEXT(".")));
		FString SoundPath = "/Game/" + GBackgroundMusicPath + "/" + SoundName;
		
		USoundWave* SoundBase = LoadObject<USoundWave>(nullptr, *SoundPath);
		if(SoundBase)
		{
			BackgroundMusics.Add(TStrongObjectPtr(SoundBase));
		}
	}
}
