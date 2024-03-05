// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/ItemBase.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
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
	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(OtherActor);
	if(IsValid(ActionCharacter))
	{
		ActionCharacter->SetOverlappingItem(this);
	}
}

void AItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherActorName = OtherActor->GetName();
	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(OtherActor);
	if(IsValid(ActionCharacter))
	{
		ActionCharacter->SetOverlappingItem(nullptr);
	}
}

