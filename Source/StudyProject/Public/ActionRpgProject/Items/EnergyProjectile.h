// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "EnergyProjectile.generated.h"

UCLASS()
class STUDYPROJECT_API AEnergyProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnergyProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastSpawnEffect(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(EditAnywhere, Category="Components", Meta=(AllowPrivateAccess))
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComponent;

	UPROPERTY(EditAnywhere, Category="Effect", meta=(AllowPrivateAccess))
	TObjectPtr<class UParticleSystem> ParticleResource;

	UPROPERTY(EditAnywhere, Category="Effect", meta=(AllowPrivateAccess))
	TObjectPtr<class USoundBase> SoundResource;

	UPROPERTY(EditAnywhere, Category="Variables", Meta=(AllowPrivateAccess))
	float Damage = 30.f;

	
};
