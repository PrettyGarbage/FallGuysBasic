// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class STUDYPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	void SetTargetActor(ACharacter* InTargetActor);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	void SetCollisionEvent();

protected:
	/* Components */
	UPROPERTY(EditAnywhere, Category="Components", Meta=(AllowPrivateAccess))
	TObjectPtr<class UBoxComponent> BoxComponent;
	UPROPERTY(EditAnywhere, Category="Components", Meta=(AllowPrivateAccess))
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere, Category="Components", Meta=(AllowPrivateAccess))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, Category="Components", Meta=(AllowPrivateAccess))
	TObjectPtr<class UArrowComponent> ArrowComponent;

	/* Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables", Meta=(AllowPrivateAccess))
	float Speed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Variables", Meta=(AllowPrivateAccess))
	float Gravity;
	UPROPERTY(VisibleAnywhere, Category="Variables", meta=(AllowPrivateAccess))
	TObjectPtr<class ACharacter> TargetActor;
};
