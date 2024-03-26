// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/SwordWeapon.h"

#include "ActionRpgProject/Interfaces/HitInterface.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Define/DefineVariables.h"


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

void ASwordWeapon::PlayEquipSound()
{
	if(IsValid(EquipSound))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
		);
	}
}

void ASwordWeapon::DisableSphereCollision()
{
	if(IsValid(SphereComponent))
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASwordWeapon::DeactivateEmbers()
{
	if(IsValid(NiagaraComponent))
	{
		NiagaraComponent->Deactivate();
	}
}

void ASwordWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* InActor, APawn* InInstigator)
{
	SetOwner(InActor);
	SetInstigator(InInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;

	PlayEquipSound();
	DisableSphereCollision();
	DeactivateEmbers();
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

void ASwordWeapon::ExecuteGetHit(FHitResult BoxHit)
{
	if(IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor()))
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

bool ASwordWeapon::IsActorSameType(AActor* OtherActor)
{
	if(!IsValid(GetOwner()) || !IsValid(OtherActor)) return false;

	return GetOwner()->ActorHasTag(GTag_Enemy) && OtherActor->ActorHasTag(GTag_Enemy);
}

void ASwordWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(IsActorSameType(OtherActor) || ItemState != EItemState::EIS_Equipped) return;
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if(IsValid(BoxHit.GetActor()) && !IsActorSameType(BoxHit.GetActor()))
	{
		UGameplayStatics::ApplyDamage(
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
			);
		
		ExecuteGetHit(BoxHit);

		CreateFields(BoxHit.ImpactPoint);
	}
}

void ASwordWeapon::BoxTrace(FHitResult& BoxHit)
{
	FVector Start = BoxTraceStart->GetComponentLocation();
	FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	for(AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		BoxHit,
		true
		);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}



