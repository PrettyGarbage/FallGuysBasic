#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName="UnEquipped"),
	ECS_Equipped UMETA(DisplayName="Equipped Sword Weapon"),
};
