// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Game/ActionPlayerState.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Game/ActionGameInstance.h"
#include "ActionRpgProject/Game/ActionPlayerSaveGame.h"
#include "Kismet/GameplayStatics.h"

AActionPlayerState::AActionPlayerState()
{
}

void AActionPlayerState::InitPlayerState()
{
	
}

void AActionPlayerState::SavePlayerState()
{
	UActionPlayerSaveGame* SaveGameInstance = NewObject<UActionPlayerSaveGame>();
	SaveGameInstance->PlayerCharacterName = GetPlayerName();
	SaveGameInstance->AllItems = AllItems;
	
	if(true == UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerCharacterName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveGame Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save Failed"));
	}
}

void AActionPlayerState::LoadPlayerState()
{
	UActionPlayerSaveGame* SaveGameInstance = Cast<UActionPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(GetPlayerName(), 0));
	if(IsValid(SaveGameInstance))
	{
		AllItems = SaveGameInstance->AllItems;
		BroadCastHandler(AllItems);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveData is not exist"));
	}
}

/*
 * 캐릭터의 BeginPlay가 PlayerState보다 늦게 호출 되기 때문에 싱글턴처럼 늘 존재하는 GameInstance와 달리
 * PlayerController나 PlayerState는 게임에 접속한 플레이어마다 생성되기 때문에 기다렸다가 이벤트를 보내주는
 * 방식으로 생각해 봄. (lyra 게임 참조)
*/
template<class T>
void AActionPlayerState::BroadCastHandler(const T& InData)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,
		[this,InData, TimerHandle]() mutable -> void
		{
			AActionCharacter* ActionCharacter = Cast<AActionCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if(IsValid(ActionCharacter))
			{
				if(OnLoadItemDataDelegate.IsBound())
				{
					OnLoadItemDataDelegate.Broadcast(InData);
					UKismetSystemLibrary::PrintString(GetWorld(), "Load Success & Broadcast");
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}
			}
		}, 0.1f, false);
}

