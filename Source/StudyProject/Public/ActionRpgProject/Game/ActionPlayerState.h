// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRpgProject/Structs/SlotStruct.h"
#include "GameFramework/PlayerState.h"
#include "ActionPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadItemDataDelegate, const FAllItems&, InAllItems);

UCLASS()
class STUDYPROJECT_API AActionPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AActionPlayerState();

	void InitPlayerState();

	FAllItems GetAllItems() const {return AllItems;}

	void SetAllItems(const FAllItems& InAllItems) {AllItems = InAllItems;}

	void SavePlayerState();

	void LoadPlayerState();

private:
	template< class T>
	void BroadCastHandler(const T& InData);

public:
	FOnLoadItemDataDelegate OnLoadItemDataDelegate;
	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="SaveData", Meta=(AllowPrivateAccess=true))
	TObjectPtr<class UActionGameInstance> ActionGameInstance;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="SaveData", Meta=(AllowPrivateAccess=true))
	FAllItems AllItems;
};
