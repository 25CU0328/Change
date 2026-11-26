// Fill out your copyright notice in the Description page of Project Settings.


#include "VineWall.h"

// 毎フレームの更新処理
void AVineWall::Tick(float DeltaTime)
{
	if (IsUesAbility)
	{
		Gimmick();
	}
}

// 対象の動物かどうかチェック
bool AVineWall::IsAnimalcheck(ETransformState etrasnformstate)
{
	bool check = false;
	// 合っているならtrueにする
	if (etrasnformstate == ETransformState::Boar)
	{
		check = true;
	}
	return check;
}

// ギミックの能力
void AVineWall::Gimmick()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy"));
	this->Destroy();
}