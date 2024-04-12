// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STUDYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	void ActiveInventoryUI();

	//Inventory
	FORCEINLINE TObjectPtr<class UUIInventory> GetInventoryWidget() const { return InventoryWidget; }
	
	FORCEINLINE TObjectPtr<class UUserHealthBar> GetHealthBarWidget() const { return HealthBarWidget; }

	UFUNCTION(BlueprintCallable)
	void DecreaseHP();

	UFUNCTION(BlueprintCallable)
	void TraceItemToPickUp();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	void OpenInventory(APlayerController* PlayerController);

	void CreateHealthBar();

	

private:
	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUIInventory> InventoryWidgetClass;

	TObjectPtr<class UUIInventory> InventoryWidget;

	UPROPERTY(EditAnywhere, Category="Widget", meta=(AllowPrivateAccess="true"))
	TSubclassOf<class UUserHealthBar> HealthBarWidgetClass;

	TObjectPtr<class UUserHealthBar> HealthBarWidget;
};
