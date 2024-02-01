// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldStatic/SLandMine.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASLandMine::ASLandMine() : bIsExploded(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyBoxComponent"));
	SetRootComponent(BodyBoxComponent);

	BodyStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyStaticMeshComponent"));
	BodyStaticMeshComponent->SetupAttachment(BodyBoxComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(BodyBoxComponent);
	ParticleSystemComponent->SetAutoActivate(false);
}

void ASLandMine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsExploded);
}

void ASLandMine::OnRep_IsExploded()
{
	if(bIsExploded)
	{
		BodyStaticMeshComponent->SetMaterial(0, ExplodedMaterial);
	}
}

// Called when the game starts or when spawned
void ASLandMine::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority()) //HasAuthority가 true면 server가 주체 
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called in Server PC.")));
	}
	else
	{
		if(GetOwner() == UGameplayStatics::GetPlayerController(this, 0)) // 0번이 owner
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called in OwningClient PC.")));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::BeginPlay() has been called in OtherClient PC.")));
		}
	}

	if(!OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ASLandMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called in Server PC.")));
	}
	else
	{
		if(GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called in OwningClient PC.")));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::EndPlay() has been called in OtherClient PC.")));
		}
	}

	if(OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.RemoveDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ASLandMine::OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::OnLandMineBeginOverlap() has been called in Server PC.")));
	}
	else
	{
		if (GetOwner() == UGameplayStatics::GetPlayerController(this, 0))
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::OnLandMineBeginOverlap() has been called in OwningClient PC.")));
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ASLandMine::OnLandMineBeginOverlap() has been called in OtherClient PC.")));
		}
	}

	if (HasAuthority() && !bIsExploded)
	{
		SpawnEffect_NetMulticast();
		bIsExploded = true;
	}
}

void ASLandMine::SpawnEffect_NetMulticast_Implementation()
{
	ParticleSystemComponent->Activate(true);

	if(IsValid(ExplodedMaterial))
	{
		BodyStaticMeshComponent->SetMaterial(0, ExplodedMaterial);
	}
}





