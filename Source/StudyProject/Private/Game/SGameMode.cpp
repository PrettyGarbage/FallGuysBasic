// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"

#include "Characters/SPlayerPawn.h"
#include "Controllers/SPlayerController.h"
#include "Game/SGameState.h"
#include "Game/SPlayerState.h"
#include "Kismet/GameplayStatics.h"

ASGameMode::ASGameMode()
{
	DefaultPawnClass = ASPlayerPawn::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	
}

void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	SGameState = GetGameState<ASGameState>();
	if(!SGameState)
	{
		return;
	}
	
	if(SGameState->MatchState != EMatchState::Waiting)
	{
		NewPlayer->SetLifeSpan(.1f);
		return;
	}
	
	ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
	if(IsValid(PlayerState))
	{
		PlayerState->InitPlayerState();
	}

	ASPlayerController* NewPlayerController = Cast<ASPlayerController>(NewPlayer);
	if(IsValid(NewPlayerController))
	{
		AlivePlayerControllers.Add(NewPlayerController);

		NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server"));
	}
	
}

void ASGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASPlayerController* ExitingPlayerController = Cast<ASPlayerController>(Exiting);
	if(IsValid(ExitingPlayerController) && INDEX_NONE != AlivePlayerControllers.Find(ExitingPlayerController))
	{
		AlivePlayerControllers.Remove(ExitingPlayerController);
		DeadPlayerControllers.Add(ExitingPlayerController);
	}
}

void ASGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);

	RemainWaitingTimerForPlaying = WaitingTime;
}

void ASGameMode::OnControllerDead(ASPlayerController* InDeadController)
{
	if(!IsValid(InDeadController) || INDEX_NONE == AlivePlayerControllers.Find(InDeadController))
	{
		return;
	}

	InDeadController->ShowLooserUI(AlivePlayerControllers.Num());

	AlivePlayerControllers.Remove(InDeadController);
	DeadPlayerControllers.Add(InDeadController);
}

void ASGameMode::OnMainTimerElapsed()
{
	SGameState = GetGameState<ASGameState>();
	if(!SGameState)
	{
		return;
	}
	
	switch (SGameState->MatchState)
	{
	case EMatchState::None:
		break;
	case EMatchState::Waiting:
		{
			FString NotificationString = FString::Printf(TEXT(""));

			if(AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
			{
				NotificationString = FString::Printf(TEXT("Wait another players for playing"));

				RemainWaitingTimerForPlaying = WaitingTime;
			}
			else
			{
				NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimerForPlaying);
				--RemainWaitingTimerForPlaying;
			}

			if(RemainWaitingTimerForPlaying == 0)
			{
				NotificationString = FString::Printf(TEXT(""));
				SGameState->MatchState = EMatchState::Playing;
			}

			NotifyToAllPlayer(NotificationString);

			break;
		}
	case EMatchState::Playing:
		{
			if(IsValid(SGameState))
			{
				SGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

				FString NotificationString = FString::Printf(TEXT("%d / %d"), SGameState->AlivePlayerControllerCount, SGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

				NotifyToAllPlayer(NotificationString);

				if(SGameState->AlivePlayerControllerCount <= 1)
				{
					SGameState->MatchState = EMatchState::End;
				}
			}

			if(SGameState->AlivePlayerControllerCount <= 1)
			{
				AlivePlayerControllers[0]->ShowWinnerUI();
				SGameState->MatchState = EMatchState::Ending;
			}
		
			break;
		}
	case EMatchState::Ending:
		{
			FString NotificationString = FString::Printf(TEXT("Waiting %d for returning to the lobby"), RemainWaitingTimerForEnding);

			NotifyToAllPlayer(NotificationString);

			--RemainWaitingTimerForEnding;

			if(RemainWaitingTimerForEnding == 0)
			{
				for(auto AliveController : AlivePlayerControllers)
				{
					AliveController->ReturnToLobby();
				}
				for(auto DeadController : DeadPlayerControllers)
				{
					DeadController->ReturnToLobby();
				}

				MainTimerHandle.Invalidate();
			}
		
			FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
			UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));
			break;
		}
	case EMatchState::End:
		break;
	default:
		break;
	}
}

void ASGameMode::NotifyToAllPlayer(const FString& NotificationString)
{
	for(const auto AlivePlayerController : AlivePlayerControllers)
	{
		if(IsValid(AlivePlayerController))
		{
			AlivePlayerController->NotificationText = FText::FromString(NotificationString);
		}

	}

	for(const auto DeadPlayerController : DeadPlayerControllers)
	{
		if(IsValid(DeadPlayerController))
		{
			DeadPlayerController->NotificationText = FText::FromString(NotificationString);
		}
	}
}


