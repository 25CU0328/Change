// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventSubsystem.h"

// サブシステムの初期化処理
void UGameEventSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp,Log,TEXT("GameEventSubsystem Initialize"));
}

// サブシステムの終了処理
void UGameEventSubsystem::Deinitialize()
{
    Super::Deinitialize();

    UE_LOG(LogTemp, Log, TEXT("GameEventSubsystem Deinitialize"));
}