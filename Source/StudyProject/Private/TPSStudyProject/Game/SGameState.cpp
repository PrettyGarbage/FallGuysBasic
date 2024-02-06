// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Game/SGameState.h"

#include "Net/UnrealNetwork.h"

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AlivePlayerControllerCount);
	DOREPLIFETIME(ThisClass, MatchState);
}
