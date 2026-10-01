#pragma once

// ゲーム内のイベントを表す列挙型
enum class EGameEvent
{
	None = -1,
	
	// 変身
	TransformStart,
	TransformComplete,
	TransformCanceled,

	// 入力
	MoveInput,
	TransformInput,
	UseAbilityInput,
};