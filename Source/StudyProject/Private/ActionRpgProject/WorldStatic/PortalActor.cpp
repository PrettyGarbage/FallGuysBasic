// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/WorldStatic/PortalActor.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APortalActor::APortalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APortalActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APortalActor::OnBoxOverlap);
}


void APortalActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsValid(OtherActor))
	{
		AActionCharacter* ActionCharacter = Cast<AActionCharacter>(OtherActor);
		if(IsValid(ActionCharacter))
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName(TransferLevelName.ToString()));
		}
	}
}

