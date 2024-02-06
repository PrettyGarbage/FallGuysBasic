// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLandMine.generated.h"

UCLASS()
class STUDYPROJECT_API ASLandMine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASLandMine();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_IsExploded();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnEffect_NetMulticast();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ASLandMine", Meta=(AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> BodyBoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ASLandMine", Meta=(AllowPrivateAccess))
	TObjectPtr<class UStaticMeshComponent> BodyStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ASLandMine", Meta=(AllowPrivateAccess))
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(ReplicatedUsing=OnRep_IsExploded, Replicated, VisibleAnywhere, BlueprintReadOnly, Category="ASLandMine", Meta=(AllowPrivateAccess))
	uint8 bIsExploded : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ASLandMine", Meta=(AllowPrivateAccess))
	TObjectPtr<class UMaterial> ExplodedMaterial;
};
