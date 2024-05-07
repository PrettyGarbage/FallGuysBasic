// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/EnergyProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AEnergyProjectile::AEnergyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnergyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AEnergyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	const FVector HitLocation = Hit.Location;

	if(IsValid(ParticleResource))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleResource, HitLocation, FRotator::ZeroRotator,
	FVector(1.f), true, EPSCPoolMethod::AutoRelease);
	}

	if(IsValid(SoundResource))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundResource, HitLocation);
	}

	if(IsValid(OtherActor))
	{
		UGameplayStatics::ApplyDamage(
			OtherActor,
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
			);
	}

	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

// Called every frame
void AEnergyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

