// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASPlayerController();

	class USHUD* GetHUDWidget() const { return HUDWidget; }

	void ToggleMenu();
	
protected:
	virtual void  SetupInputComponent() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= ASPlayerController, meta=(AllowPrivateAccess))
	TSubclassOf<class UUserWidget> MenuUIClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= ASPlayerController, meta=(AllowPrivateAccess))
	TObjectPtr<class UUserWidget> MenuUIInstance;

	bool bIsMenuOn = false;

private:
	UPROPERTY();
	TObjectPtr<class USHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASPlayerController", Meta = (AllowPrivateAccess));
	TSubclassOf<class USHUD> HUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = USTPSAnimInstance, meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> CrossHairUIClass;
};
