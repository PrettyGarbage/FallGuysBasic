// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_None,
	EIS_Equipped,
};

UCLASS()
class STUDYPROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<class UStaticMeshComponent> ItemMesh;
	
	EItemState ItemState = EItemState::EIS_None;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> SphereComponent;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemBase", meta = (AllowPrivateAccess))
	float RunningTime;
	
};
