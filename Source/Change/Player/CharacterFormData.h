#pragma once

#include "CoreMinimal.h"

#include "Change/ETransformState.h"
#include "CharacterFormData.generated.h"


// アニメション関連クラス
class UPaperZDAnimInstance;

// 変身に必要なデータを管理するための構造体
USTRUCT(BlueprintType)
struct FCharacterFormData
{
	GENERATED_BODY()

	// 使用するアニメションインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UPaperZDAnimInstance> AnimationInstance;

	// --------------
	// 移動関連
	// --------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed;	// 最大速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration;	// 加速度 (cm/s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Deceleration;	// 加速していない時の減速
	

	// --------------
	// 衝突判定
	// --------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider")
	float ColliderRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider")
	float ColliderHalfHeight;
};