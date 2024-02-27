// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/SwordWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASwordWeapon::ASwordWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponBox->SetCollisionResponseToChannels(ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void ASwordWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;

	if(IsValid(EquipSound))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
			);
	}

	if(IsValid(SphereComponent))
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASwordWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void ASwordWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &ASwordWeapon::OnBoxOverlap);
}

void ASwordWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ASwordWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ASwordWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FVector Start = BoxTraceStart->GetComponentLocation();
	FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHit;
	
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f,5.f,5.f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true
		);
	
}



