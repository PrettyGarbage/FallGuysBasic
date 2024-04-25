// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRpgProject/HUD/UserHealthBar.h"

#include "ActionRpgProject/Characters/ActionCharacter.h"
#include "ActionRpgProject/Components/AttributeComponent.h"
#include "ActionRpgProject/HUD/Heart.h"
#include "Components/WrapBox.h"

void UUserHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	UpdateHealthBar();
}

//위젯 생성후 데이터 기반으로 다시 채워나가는 방식으로 변경
void UUserHealthBar::UpdateHealthBar()
{
	if(!IsValid(HealthBarWrapBox)) return;
	
	AActionCharacter* ActionCharacter = Cast<AActionCharacter>(GetOwningPlayerPawn());
	if(IsValid(ActionCharacter))
	{
		UAttributeComponent* AttributeComponent = ActionCharacter->GetComponentByClass<UAttributeComponent>();

		if(IsValid(AttributeComponent))
		{
			float CurrentHealth = AttributeComponent->GetHealthValue();
			int32 MaxHealth = AttributeComponent->GetMaxHealthValue();
			int32 StartIndex = HealthBarWrapBox->GetChildrenCount();

			//만약에 MaxHealth 데이터가 업데이트 되어 증가 된다면 추가해준다.
			for(int32 i = StartIndex; i < MaxHealth; i++)
			{
				AddHealthBar();
			}
			
			for(int32 i = 0; i < HeartArray.Num(); i++)
			{
				if(IsValid(HeartArray[i]))
				{
					const bool bIsHalfHeart = i + 0.5f == CurrentHealth;
					const bool bIsFullHeart = i < CurrentHealth;
					EHeartStatus HeartStatus = bIsHalfHeart ? EHeartStatus::EHS_Half :
						bIsFullHeart? EHeartStatus::EHS_Full : EHeartStatus::EHS_Empty;

					if(HeartArray[i]->GetHealthStatus() == HeartStatus)
					{
						continue;
					}
					HeartArray[i]->SetHealth(bIsHalfHeart ? 0.5 : bIsFullHeart ? 1 : 0);
				}
			}
		}
	}
}

void UUserHealthBar::AddHealthBar()
{
	UHeart* Heart = CreateWidget<UHeart>(GetWorld(), HeartWidget);
	HeartArray.Add(Heart);
	if(IsValid(HealthBarWrapBox))
	{
		HealthBarWrapBox->AddChild(Heart);
	}
}
