// Fill out your copyright notice in the Description page of Project Settings.


#include "Waterway.h"
#include "Components/BoxComponent.h"

// Sets default values
AWaterway::AWaterway()
	:bPlayerAbility(false)
{
	// ルートに置くベースオブジェクト生成
	pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = pBase;

	// コリジョン判定用ボックスコンポ―ネット生成
	pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	pBoxComp->SetupAttachment(RootComponent);
}

// 毎フレームの更新処理
void AWaterway::Tick(float DeltaTime)
{
	if (IsUesAbility)
	{
		Gimmick();
	}
}

// 対象の動物かどうかチェック
bool AWaterway::IsAnimalcheck(ETransformState etrasnformstate)
{
	bool check = false;
	// 合っているならtrueにする
	if (etrasnformstate == ETransformState::Elephant)
	{
		check = true;
	}
	return check;
}

// ギミックの能力
void AWaterway::Gimmick()
{
}