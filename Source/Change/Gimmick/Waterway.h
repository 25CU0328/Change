// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "Waterway.generated.h"

//前方宣言
class UBoxComponent;

/**
 * 
 */
UCLASS()
class CHANGE_API AWaterway : public AGimmickBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterway();

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

	// 対象の動物かどうかチェック
	bool IsAnimalcheck(ETransformState etrasnformstate)override;

private:
	// ギミックの能力
	void Gimmick()override;

protected:
	// 接触用のコリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* pBoxComp;

	// ベースコンポ―ネット
	UPROPERTY()
	USceneComponent* pBase;

};
