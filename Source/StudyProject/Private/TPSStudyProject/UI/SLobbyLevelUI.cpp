// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/UI/SLobbyLevelUI.h"

#include "SPlayerCharacterSettings.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "TPSStudyProject/Controllers/SUIPlayerController.h"
#include "TPSStudyProject/Game/SGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TPSStudyProject/Game/SPlayerSaveGame.h"

void USLobbyLevelUI::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter* PlayerCharacter = Cast<ACharacter>(GetOwningPlayerPawn());
	CurrentSkeletalMeshComponent = PlayerCharacter->GetMesh();

	RedTeamButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnRedTeamButtonClicked);
	BlueTeamButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnBlueTeamButtonClicked);
	SubmitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnSubmitButtonClicked);
}

void USLobbyLevelUI::OnRedTeamButtonClicked()
{
	SetTeam(SelectedTeamType = ETeamType::Red);
}

void USLobbyLevelUI::OnBlueTeamButtonClicked()
{
	SetTeam(SelectedTeamType = ETeamType::Blue);
}

void USLobbyLevelUI::OnSubmitButtonClicked()
{
	PlayerName = EditPlayerName->GetText().ToString();
	if(PlayerName.Len() <= 0 || 10 <= PlayerName.Len())
	{
		return;
	}

	SaveInitializeSaveData();

	ASUIPlayerController* PlayerController = GetOwningPlayer<ASUIPlayerController>();
	if(IsValid(PlayerController))
	{
		FText ServerIP = EditServerIP->GetText();
		PlayerController->JoinServer(ServerIP.ToString());
	}
}

void USLobbyLevelUI::SaveInitializeSaveData()
{
	USPlayerSaveGame* PlayerStateSave = NewObject<USPlayerSaveGame>();
	PlayerStateSave->PlayerCharacterName = PlayerName;
	PlayerStateSave->CurrentLevel = 1;
	PlayerStateSave->CurrentEXP = 0.f;
	PlayerStateSave->TeamType = SelectedTeamType;

	if (true == UGameplayStatics::SaveGameToSlot(PlayerStateSave, TEXT("PlayerCharacter0"), 0))
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Saved.")));
	}
}


void USLobbyLevelUI::SetTeam(ETeamType InSelectedTeam)
{
	const int CastingNum = static_cast<int32>(InSelectedTeam) - 1;

	if(!(CastingNum == 0 or CastingNum == 1))
	{
		UKismetSystemLibrary::PrintString(GetWorld(),FString(TEXT("입력값이 잘못됨 체크 요함.")));
		return;
	}
	
	const USPlayerCharacterSettings* CharacterSettings = GetDefault<USPlayerCharacterSettings>();
	if(IsValid(CharacterSettings))
	{
		FSoftObjectPath SkeletalMeshAssetPath = CharacterSettings->PlayerCharacterMeshPaths[CastingNum];

		if(USGameInstance* SGameInstance = GetWorld()->GetGameInstance<USGameInstance>())
		{
			if(USkeletalMesh* Asset = SGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(SkeletalMeshAssetPath))
			{
				CurrentSkeletalMeshComponent->SetSkeletalMesh(Asset);
			}
		}
	}
}
