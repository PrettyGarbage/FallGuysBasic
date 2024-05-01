// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Controller/ActionPlayerController.h"

#include "ActionRpgProject/Subsystems/SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void AActionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitBackgroundMusic();
}

void AActionPlayerController::InitBackgroundMusic()
{
	//사운드 매니저 서브시스템을 가져옴
	USoundManagerSubsystem* SoundManager = GetGameInstance()->GetSubsystem<USoundManagerSubsystem>();
	if(SoundManager)
	{
		//랜덤으로 배경음악을 가져옴
		TStrongObjectPtr<USoundWave> BackgroundMusic = SoundManager->GetBackgroundMusic();
		if(BackgroundMusic)
		{
			//배경음악을 재생
			UGameplayStatics::PlaySound2D(GetWorld(), BackgroundMusic.Get());
		}
	}
}
