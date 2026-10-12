#pragma once

// プレイヤーの向きを表す列挙型
UENUM(BlueprintType)
enum class EPlayerDirection : uint8
{
	Up		UMETA(DisplayName = "Up"),
	Down	UMETA(DisplayName = "Down"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};