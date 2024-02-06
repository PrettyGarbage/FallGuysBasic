// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSStudyProject/Examples/SPigeon.h"

USPigeon::USPigeon()
{
	Name = TEXT("USPigeon Object CDO");
}

void USPigeon::Fly()
{
	UE_LOG(LogTemp, Log, TEXT("%s is now flying"), *Name);
}

void USPigeon::Serialize(FArchive& Ar)
{
	UObject::Serialize(Ar);

	Ar << Name;
	Ar << ID;
}
