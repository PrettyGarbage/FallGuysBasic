#pragma once
#include "CoreMinimal.h"
#include "SlotStruct.generated.h"


class STUDYPROJECT_API SlotStruct
{
public:
	SlotStruct();
	~SlotStruct();
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Sword UMETA(DisplayName = "Sword"),
	Shield UMETA(DisplayName = "Shield"),
	Eatable UMETA(DisplayName = "Eatable"),
};

//Atomic은 멀티스레딩 환경에서 안전한 동시 접근을 보장한다고 함.
/*
* FInventorySlot 구조체는 인벤토리 슬롯을 나타내는 구조체입니다.
*/
USTRUCT(Atomic, BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
};

USTRUCT(Atomic, BlueprintType)
struct FAllItems
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlot> Swords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlot> Shields;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlot> Eatables;
};

USTRUCT(Atomic, BlueprintType)
struct FEnemyTableRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	FEnemyTableRow()
	{
		
	}
	FEnemyTableRow(FString InName, float InMaxHP)
	{
		Name = InName;
		MaxHP = InMaxHP;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;
};
