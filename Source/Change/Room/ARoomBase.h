// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARoomBase.generated.h"

// シンプルな部屋ベースクラス
UCLASS()
class CHANGE_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	ARoomBase();

protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

public:
	// 毎フレームの処理（必要であれば有効化してください）
	virtual void Tick(float DeltaTime) override;
};
