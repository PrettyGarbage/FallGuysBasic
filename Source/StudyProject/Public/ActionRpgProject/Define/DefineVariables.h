#pragma once

#pragma region Tags

static FName GTag_Player  = "PlayerCharacter";
static FName GTag_Enemy = "Enemy";

#pragma endregion

#pragma region SocketNames

static FName GRightHandSocket = "RightHandSocket";
static FName GWeaponSocket = "WeaponSocket";
static FName GSpineSocket = "SpineSocket";

#pragma endregion

#pragma region Strings

static FString GAttackString = "Attack";

#pragma endregion

#pragma region MontageSections

static FName GEquipSection = "Equip";
static FName GUnEquipSection = "Unequip";
static FName GFromFront = "FromFront";
static FName GFromLeft = "FromLeft";
static FName GFromRight = "FromRight";
static FName GFromBack = "FromBack";
static FName GDodge = "Dodge";

#pragma endregion

#pragma region MotionWarp Target Name

static FName GTranslationTarget = "TranslationTarget";
static FName GRotationTarget = "RotationTarget";

#pragma endregion

#pragma region Use Stamina Value

static float GUseDodgeStamina = 20.f;

static float GUseAttackStamina = 10.f;

#pragma endregion 

#pragma region BlackboardKeys

static FName GStartPatrolPositionKey = "StartPatrolPosition";
static FName GEndPatrolPositionKey = "EndPatrolPosition";
static FName GTargetActorKey = "TargetActor";
static FName GIsAliveKey = "IsAlive";

#pragma endregion

#pragma region Folder Paths

static FString GBackgroundMusicPath = "Assets/Audio/BGM";

#pragma endregion