#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName="UnEquipped"),
	ECS_Equipped UMETA(DisplayName="Equipped Sword Weapon"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_None UMETA(DisplayName="None"),
	EAS_Attacking UMETA(DisplayName="Attacking"),
	EAS_Equipping UMETA(DisplayName="Equipping"),
};
