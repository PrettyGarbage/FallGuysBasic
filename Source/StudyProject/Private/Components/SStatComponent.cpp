// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SStatComponent.h"

#include "Characters/SCharacter.h"
#include "Game/SGameInstance.h"
#include "Game/SPlayerState.h"

// Sets default values for this component's properties
USStatComponent::USStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = false;
}

void USStatComponent::SetMaxHP(float InMaxHP)
{
	if(OnMaxHPChangeDelegate.IsBound())
	{
		OnMaxHPChangeDelegate.Broadcast(MaxHP, InMaxHP);
	}
	MaxHP = FMath::Clamp<float>(InMaxHP, 0.f, FLT_MAX);
}

void USStatComponent::SetCurrentHP(float InCurrentHP)
{
	if(OnCurrentHPChangeDelegate.IsBound())
	{
		OnCurrentHPChangeDelegate.Broadcast(CurrentHP, InCurrentHP);
	}

	CurrentHP = FMath::Clamp<float>(InCurrentHP, 0.f, MaxHP);

	if(CurrentHP <= KINDA_SMALL_NUMBER && OnOutOfCurrentHPDelegate.IsBound())
	{
		OnOutOfCurrentHPDelegate.Broadcast();
		CurrentHP = 0.f;
	}
}

void USStatComponent::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if(IsValid(GameInstance))
	{
		if(GameInstance->GetCharacterStatDataTable() != nullptr && GameInstance->GetCharacterStatDataTable()->GetRowMap().Num() > 0)
		{
			FSStatTableRow* StatTableRow = GameInstance->GetCharacterStatDataTableRow(1);
			if(StatTableRow != nullptr)
			{
				SetMaxHP(StatTableRow->MaxHP);
				SetCurrentHP(MaxHP);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Not enuough data in CharacterStatDataTable."));
		}
	}

	ASCharacter* OwnerPlayerCharacter = Cast<ASCharacter>(GetOwner());
	if (IsValid(OwnerPlayerCharacter))
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(OwnerPlayerCharacter->GetPlayerState());
		if (IsValid(PlayerState))
		{
			if (!PlayerState->OnCurrentLevelChangedDelegate.IsAlreadyBound(this, &ThisClass::OnCurrentLevelChanged))
			{
				PlayerState->OnCurrentLevelChangedDelegate.AddDynamic(this, &ThisClass::OnCurrentLevelChanged);
			}
		}
	}
}

void USStatComponent::OnCurrentLevelChanged(int32 InPrevCurrentLevel, int32 InNewCurrentLevel)
{
	SetMaxHP(GameInstance->GetCharacterStatDataTableRow(InNewCurrentLevel)->MaxHP);
	SetCurrentHP(GameInstance->GetCharacterStatDataTableRow(InNewCurrentLevel)->MaxHP);
}




