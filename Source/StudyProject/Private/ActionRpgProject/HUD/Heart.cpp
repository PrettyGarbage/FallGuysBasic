// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/Heart.h"

#include "Components/Image.h"

void UHeart::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UHeart::NativeConstruct()
{
	Super::NativeConstruct();

}

void UHeart::SetHealth(float Amount)
{
	if (Amount == 1.0f)
	{
		HeartImage->SetBrushFromTexture(MaxHeartTexture);
		HeartStatus = EHeartStatus::EHS_Full;
	}
	else if (Amount == 0.5f)
	{
		HeartImage->SetBrushFromTexture(HalfHeartTexture);
		HeartStatus = EHeartStatus::EHS_Half;
	}
	else
	{
		HeartImage->SetBrushFromTexture(EmptyHeartTexture);
		HeartStatus = EHeartStatus::EHS_None;
	}
}

EHeartStatus UHeart::GetHealthStatus()
{
	return HeartStatus;
}
