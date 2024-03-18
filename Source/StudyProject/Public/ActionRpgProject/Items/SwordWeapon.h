// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "SwordWeapon.generated.h"

UCLASS()
class STUDYPROJECT_API ASwordWeapon : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASwordWeapon();
	void PlayEquipSound();
	void DisableSphereCollision();
	void DeactivateEmbers();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* InActor, APawn* InInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	FORCEINLINE TObjectPtr<class UBoxComponent> GetWeaponBox() const { return WeaponBox; }

	
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	void ExecuteGetHit(FHitResult BoxHit);
	bool IsActorSameType(AActor* OtherActor);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
	
private:
	
	void BoxTrace(FHitResult& BoxHit);
	
	
public:
	UPROPERTY()
	TArray<TObjectPtr<class AActor>> IgnoreActors;
	
private:
	UPROPERTY(EditAnywhere, Category="Weapon Properties", meta=(AllowPrivateAccess="true"))
	FVector BoxTraceExtent = FVector(5.f);
	
	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	TObjectPtr<class USoundBase> EquipSound;

	UPROPERTY(VisibleAnywhere, Category="Weapon Proerties")
	TObjectPtr<class UBoxComponent> WeaponBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USceneComponent> BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category="Weapon Properties")
	float Damage = 20.f;
};
