#pragma once

#include "CoreMinimal.h"

// ゲーム内のイベントを表す列挙型
UENUM(BlueprintType)
enum class EGameEvent : uint8
{
	None = 0,

	Transform, // 変身
	UseAbility, // 能力使用

	RoomClear, // ルームクリア
	StageClear,
	TransitionLevel, // 別のレベルに遷移する

	SwitchPressed,	// スウィッチが押された
	SwitchReleased, // スウィッチが離された
	// -------------
	// UI Event
	// --------------
	StartGameButtonPressed,	// ゲーム開始ボタンが押された
	EndGameButtonPressed,	// ゲーム終了ボタンが押された

	ResumeButtonPressed,// ゲーム再開ボタンが押された
	QuitButtonPressed,	// ステージを諦めるボタンが押された
};