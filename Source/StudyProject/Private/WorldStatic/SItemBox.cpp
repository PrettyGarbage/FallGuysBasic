// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldStatic/SItemBox.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASItemBox::ASItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASItemBox::OnOverlapBegin);
	BoxComponent->SetCollisionProfileName(FName("SItemBox"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMesh(TEXT("오브젝트 패스"));
	if (BoxMesh.Object)
	{
		StaticMeshComponent->SetStaticMesh(BoxMesh.Object);
	}
	*/

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->SetAutoActivate(false);
	/*
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectTemplate(TEXT("오브젝트 패스"));
	if (EffectTemplate.Object)
	{
		ParticleSystemComponent->SetTemplate(EffectTemplate.Object);
		ParticleSystemComponent->bAutoActivate = false;
	}
	*/
}

void ASItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ParticleSystemComponent->Activate(true);
	StaticMeshComponent->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	ParticleSystemComponent->OnSystemFinished.AddDynamic(this, &ASItemBox::OnEffectFinished);
}

void ASItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}




