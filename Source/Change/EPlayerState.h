#pragma once

// プレイヤーの状態を表す列挙型
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,		
	Move,		
	Transform,	
	UseAbility,
};