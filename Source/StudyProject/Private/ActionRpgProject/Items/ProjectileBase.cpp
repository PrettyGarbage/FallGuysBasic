// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/Items/ProjectileBase.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Controller/ActionPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AProjectileBase::AProjectileBase() : bIsHit(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->ProjectileGravityScale = Gravity;
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
}

void AProjectileBase::SetTargetActor(ACharacter* InTargetActor)
{
	if(!IsValid(InTargetActor)) return;

	if(ProjectileMovementComponent)
	{
		//InTargetActor의 방향 벡터를 구한다.
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), InTargetActor->GetActorLocation());
		SetActorRotation(Direction.Rotation());
		ProjectileMovementComponent->Velocity = Direction * Speed;
	}
}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AProjectileBase, bIsHit, COND_None);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetCollisionEvent();

	bReplicates = true;
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(BoxComponent->OnComponentHit.IsAlreadyBound(this, &AProjectileBase::OnHit))
	{
		BoxComponent->OnComponentHit.RemoveDynamic(this, &AProjectileBase::OnHit);
	}
	
}

void AProjectileBase::SetCollisionEvent()
{
	if(IsValid(BoxComponent))
	{
		BoxComponent->MoveIgnoreActors.Add(GetOwner());
		BoxComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(HasAuthority())
	{
		bIsHit = true;
		Destroy();
	}
}

void AProjectileBase::OnRep_IsDestroyed()
{
	if(bIsHit)
	{
		Destroy();
	}
}





