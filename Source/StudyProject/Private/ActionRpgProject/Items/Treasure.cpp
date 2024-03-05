// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/Treasure.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATreasure::ATreasure()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(OtherActor);
	if(IsValid(ActionCharacter))
	{
		if(IsValid(PickSound))
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickSound,
				GetActorLocation()
				);
		}
		Destroy();
	}
}
