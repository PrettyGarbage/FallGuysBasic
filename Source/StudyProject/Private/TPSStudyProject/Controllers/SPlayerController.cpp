// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Controllers/SPlayerController.h"

#include "TPSStudyProject/Characters/SCharacter.h"
#include "TPSStudyProject/Components/SStatComponent.h"
#include "TPSStudyProject/Game/SPlayerState.h"
#include "TPSStudyProject/UI/SHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TPSStudyProject/Game/SGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TPSStudyProject/UI/SGameResultWidget.h"

ASPlayerController::ASPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASPlayerController::ToggleMenu()
{
	UE_LOG(LogTemp, Log, TEXT("Log Message"));
	if(!bIsMenuOn)
	{
		MenuUIInstance->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(MenuUIInstance->GetCachedWidget());
		SetInputMode(Mode);

		bShowMouseCursor = true;
	}
	else
	{
		MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);

		bShowMouseCursor = false;
	}

	bIsMenuOn = !bIsMenuOn;
	SetPause(bIsMenuOn);
}

void ASPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ASPlayerController::OnOwningCharacterDead()
{
	ASGameMode* GameMode = Cast<ASGameMode>(UGameplayStatics::GetGameMode(this));
	if(HasAuthority() && IsValid(GameMode))
	{
		GameMode->OnControllerDead(this);
	}
}

void ASPlayerController::ShowWinnerUI_Implementation()
{
	if(!HasAuthority())
	{
		if(IsValid(WinnerUIClass))
		{
			USGameResultWidget* WinnerUI = CreateWidget<USGameResultWidget>(this, WinnerUIClass);
			if(IsValid(WinnerUI))
			{
				WinnerUI->AddToViewport(3);
				WinnerUI->RankingText->SetText(FText::FromString(TEXT("#01")));

				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(WinnerUI->GetCachedWidget());
				SetInputMode(Mode);

				bShowMouseCursor = true;
			}
		}
	}
}

void ASPlayerController::ShowLooserUI_Implementation(int32 InRanking)
{
	if(!HasAuthority())
	{
		if(IsValid(LooserUIClass))
		{
			USGameResultWidget* LooserUI = CreateWidget<USGameResultWidget>(this, LooserUIClass);
			if(IsValid(LooserUI))
			{
				LooserUI->AddToViewport(3);
				LooserUI->RankingText->SetText(FText::FromString(TEXT("#01")));

				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(LooserUI->GetCachedWidget());
				SetInputMode(Mode);

				bShowMouseCursor = true;
			}
		}
	}
}

void ASPlayerController::ReturnToLobby_Implementation()
{
	if(!HasAuthority())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Loading")), true, FString(TEXT("NextLevel=Lobby?Saved=false")));
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if(HasAuthority()) //서버에선 UI 안띄움
	{
		return;
	}

	if(IsValid(HUDWidgetClass))
	{
		HUDWidget = CreateWidget<USHUD>(this, HUDWidgetClass);
		if(IsValid(HUDWidget))
		{
			HUDWidget->AddToViewport();

			ASCharacter* SCharacter = GetPawn<ASCharacter>();
			if(IsValid(SCharacter))
			{
				USStatComponent* StatComponent = SCharacter->GetStatComponent();
				if(IsValid(StatComponent))
				{
					HUDWidget->BindStatComponent(StatComponent);
				}
			}

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() -> void
			{
				ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
				if(IsValid(SPlayerState))
				{
					HUDWidget->BindPlayerState(SPlayerState);
				}
			}), .5f, false); //inRate를 0으로 해버리면 nullptr이 넘어가는 경우 있음
		}
	}

	if(IsValid(MenuUIClass))
	{
		MenuUIInstance = CreateWidget<UUserWidget>(this, MenuUIClass);
		if(IsValid(MenuUIInstance))
		{
			MenuUIInstance->AddToViewport(3);

			MenuUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if(IsValid(CrossHairUIClass))
	{
		UUserWidget* CrossHairUI = CreateWidget<UUserWidget>(this, CrossHairUIClass);
		if(IsValid(CrossHairUI))
		{
			CrossHairUI->AddToViewport(1);
			CrossHairUI->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if(IsValid(NotificationTextUIClass))
	{
		UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIClass);
		if(IsValid(NotificationTextUI))
		{
			NotificationTextUI->AddToViewport(1);
			NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
