// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlowSubsystem.h"
#include "Change/Event/GameEventSubsystem.h"
#include "Kismet/GameplayStatics.h"

// サブシステムの初期化処理
void UGameFlowSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // UEにイベントシステムを利用すると知らせる
    Collection.InitializeDependency<UGameEventSubsystem>();

    // イベントサブシステムを取得する
    auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

    // 取得できない場合
    if (!EventSubsystem)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("GameEventSubsystem is unavailable.")
        );

        return;
    }
    // -----------------------
    // コールバックの登録
    // -----------------------
    EventSubsystem->Register<UGameFlowSubsystem>(
        EGameEvent::StartGameButtonPressed,
        this,
        &UGameFlowSubsystem::OnGameStart
    );

    EventSubsystem->Register<UGameFlowSubsystem>(
        EGameEvent::EndGameButtonPressed,
        this,
        &UGameFlowSubsystem::OnGameEnd
    );

    UE_LOG(LogTemp, Log, TEXT("GameFlowSubsystem Initialize"));
}

// サブシステムの終了処理
void UGameFlowSubsystem::Deinitialize()
{
    Super::Deinitialize();

    UE_LOG(LogTemp, Log, TEXT("GameFlowSubsystem Deinitialize"));
}

// ゲーム開始に関するイベントが送られたら
void UGameFlowSubsystem::OnGameStart(const FString& EventMessage)
{
    UE_LOG(LogTemp, Log, TEXT("OnGameStart Receive Event"));
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't Get World!"));
        return;
    }

    FName LevelName(EventMessage);
    UGameplayStatics::OpenLevel(World, LevelName);
}
// ゲーム終了に関するイベントが送られたら
void UGameFlowSubsystem::OnGameEnd(const FString& EventMessage)
{
    UE_LOG(LogTemp, Log, TEXT("OnGameEnd Receive Event"));
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't Get World!"));
        return;
    }

    // プレイヤーコントローラーを取得する
    auto* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController)
    {
        return;
    }

    // ゲームを終了する
    UKismetSystemLibrary::QuitGame(
        World,
        PlayerController,
        EQuitPreference::Quit,
        false
    );
}