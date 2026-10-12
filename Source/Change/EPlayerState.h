
#pragma once

// プレイヤーの状態を表す列挙型
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Moving	UMETA(DisplayName = "Moving"),
};