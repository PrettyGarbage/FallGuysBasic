// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSStudyProject/Game/SPlayerSaveGame.h"
#include "SLobbyLevelUI.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USLobbyLevelUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnRedTeamButtonClicked();
	UFUNCTION()
	void OnBlueTeamButtonClicked();
	UFUNCTION()
	void OnSubmitButtonClicked();

	void SaveInitializeSaveData();
	
	void SetTeam(ETeamType InSelectedTeam);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> RedTeamButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> BlueTeamButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UEditableText> EditPlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> SubmitButton;

	TWeakObjectPtr<class USkeletalMeshComponent> CurrentSkeletalMeshComponent;

	FString PlayerName = TEXT("DefaultPlayerName");
	
	ETeamType SelectedTeamType = ETeamType::Red;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = USLobbyLevelUI, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UEditableText> EditServerIP;
};
