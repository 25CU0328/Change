// Fill out your copyright notice in the Description page of Project Settings.

#include "ARoomBase.h"


// コンストラクタ
ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

// ゲーム開始時の処理
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();

}

// ゲーム終了時処理
void ARoomBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// 毎フレームの処理
void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
