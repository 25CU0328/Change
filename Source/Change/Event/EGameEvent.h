#pragma once

// ゲーム内のイベントを表す列挙型
enum class EGameEvent
{
	None = -1,


	Transform, // 変身
	UseAbility, // 能力使用

	RoomClear, // ルームクリア
	
	// -------------
	// UI Event
	// --------------
	MainMenu_GameStartButtonPressed,	// メインメニュー開始ボタン
	MainMenu_GameEndButtonPressed,		// メインメニューゲーム終了ボタン

	InStageMenu_GameResumeButtonPressed,// ステージシーンでゲーム再開ボタン
	InStageMenu_GameEndButtonPressed,		// ステージシーンでゲーム終了ボタン
};