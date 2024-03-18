﻿#pragma once

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
	EAS_HitReact UMETA(DisplayName="HitReact"),
	EAS_Attacking UMETA(DisplayName="Attacking"),
	EAS_Equipping UMETA(DisplayName="Equipping"),
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Death1 UMETA(DisplayName="Death1"),
	EDP_Death2 UMETA(DisplayName="Death2"),
	EDP_Death3 UMETA(DisplayName="Death3"),
	EDP_Death4 UMETA(DisplayName="Death4"),
	EDP_Death5 UMETA(DisplayName="Death5"),
	EDP_MAX UMETA(DisplayName="Default MAX"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_None UMETA(DisplayName="None"),
	EES_Patrolling UMETA(DisplayName="Patrolling"),
	EES_Chasing UMETA(DisplayName="Chasing"),
	EES_Attacking UMETA(DisplayName="Attacking"),
	EES_Engaged UMETA(DisplayName="Engaged"),
	EES_Dead UMETA(DisplayName="Dead"),
};
