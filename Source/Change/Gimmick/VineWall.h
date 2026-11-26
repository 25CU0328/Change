// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "VineWall.generated.h"

/**
 * 
 */
UCLASS()
class CHANGE_API AVineWall : public AGimmickBase
{
	GENERATED_BODY()

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

	// 対象の動物かどうかチェック
	bool IsAnimalcheck(ETransformState etrasnformstate)override;

private:

	// ギミックの能力
	void Gimmick()override;

};
