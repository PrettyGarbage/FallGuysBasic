// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/ItemBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "ActionRpgProject/Interfaces/PickUpInterface.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToChannels(ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SetRootComponent(ItemMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnSphereEndOverlap);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ItemState == EItemState::EIS_None)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += DeltaTime * 100.f;
		SetActorRotation(NewRotation);
	}

}

void AItemBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if(PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(this);
	}
}

void AItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = OtherActor->GetName();
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if(PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(nullptr);
	}
}

