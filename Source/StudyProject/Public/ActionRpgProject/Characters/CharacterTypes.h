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

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName="Alive"),
	EDP_Death1 UMETA(DisplayName="Death1"),
	EDP_Death2 UMETA(DisplayName="Death2"),
	EDP_Death3 UMETA(DisplayName="Death3"),
	EDP_Death4 UMETA(DisplayName="Death4"),
	EDP_Death5 UMETA(DisplayName="Death5"),
};
