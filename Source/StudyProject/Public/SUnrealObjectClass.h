// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" //원래 EngineMinimal.h 이었는데 UE5 좀더 최소화 함 그리고 빌드 속도도 향상
#include "UObject/NoExportTypes.h"
#include "SUnrealObjectClass.generated.h"

/**
 * 
 */
UCLASS()
class STUDYPROJECT_API USUnrealObjectClass : public UObject
{
	GENERATED_BODY()

public:
	USUnrealObjectClass();

	UFUNCTION()
	void HelloUnreal();

	const FString& GetName() const { return Name; }

public:
	UPROPERTY()
	FString Name;
};
