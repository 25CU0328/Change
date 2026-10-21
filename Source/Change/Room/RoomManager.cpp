// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomManager.h"
#include "RoomBase.h"

// プレイヤークラスを取得するためインクルード
#include "Kismet/GameplayStatics.h"
#include "Change/MainCharacter.h"

// イベントを利用するためインクルード
#include "Change/Event/GameEventSubsystem.h"

// コンストラクター
ARoomManager::ARoomManager()
{
    CurrentRoom = nullptr;
    CurrentDataIndex = 0;
}

// ゲーム開始時の処理
void ARoomManager::BeginPlay()
{
	Super::BeginPlay();
	
    if (!GetGameInstance())
    {
        UE_LOG(LogTemp, Warning, TEXT("Can't Get GameInstance"));
        return;
    }
    auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
    if (!EventSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameEventSubsystem not available"));
        return;
    }

    // イベントを登録する
    EventSubsystem->Register<ARoomManager>(
        EGameEvent::RoomClear,
        this,
        &ARoomManager::OnRoomCleared
    );

    EventSubsystem->Register<ARoomManager>(
        EGameEvent::RoomReset,
        this,
        &ARoomManager::OnResetRoom
    );


    Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

    if (Player == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player not found."));
        return;
    }

    GenerateRoom();
}

// プレイヤーを取得する
AMainCharacter* ARoomManager::GetPlayer()
{
    return Player.Get();
}

// ルームがクリアされた時呼び出される関数
void ARoomManager::OnRoomCleared(const FString& EventMessage)
{
    if (IsAllRoomCleared())
    {
        auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
        if (!EventSubsystem)
        {
            UE_LOG(LogTemp, Warning, TEXT("GameEventSubsystem not available"));
            return;
        }

        // イベントを発送
        EventSubsystem->SendEvent(EGameEvent::StageClear, "");

        return;
    }

	// 現在ルームを削除
	RemoveCurrentRoom();

    // ルーム配列の番号を1増やす
	CurrentDataIndex += 1;
    // データ内のルーム番号を取得する
    CurrentDataArrayIndex = FMath::RandRange(0, RoomArrayDatas[CurrentDataIndex].Rooms.Num() - 1);

    // 新しいルームを生成する
	GenerateRoom();
}

// ルームをリセットしたい時呼び出されたメソッド
void ARoomManager::OnResetRoom(const FString& EventMessage)
{
    UE_LOG(LogTemp, Warning, TEXT("OnResetRoom"));
    // 現在ルームを削除する
    RemoveCurrentRoom();
    // ルームを生成する
    GenerateRoom();
}

// ルームが全部クリアされたか
bool ARoomManager::IsAllRoomCleared()
{
    return CurrentDataIndex >= RoomArrayDatas.Num();
}

// ルームを生成する
void ARoomManager::GenerateRoom()
{
    // 現在ルームが存在する場合、削除する
    if (CurrentRoom)
    {
        RemoveCurrentRoom();
    }

	// 目標ルーム配列を表す変数
    const auto& RoomArrayData = RoomArrayDatas[CurrentDataIndex];

    // 配列はヌルまたは空の場合、処理しない
    if (RoomArrayData.Rooms.IsEmpty())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Room%d array is empty!"),
            CurrentDataIndex
        );
        return;
    }
    
    

    // 選択されたルームを取得する
    TSubclassOf<ARoomBase> SelectedRoom = RoomArrayData.Rooms[CurrentDataArrayIndex];

    // 選択されたルームがヌルの場合、処理しない
    if (!SelectedRoom)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("RoomNuber %d, in RoomArray%d hasn't been set."),
            CurrentDataArrayIndex,
            CurrentDataIndex
        );
        return;
    }

    // ルームのアクターを生成する
    FActorSpawnParameters SpawnParameters;

    // ワールドに生成して、CurrentRoomに代入する
    CurrentRoom = GetWorld()->SpawnActor<ARoomBase>(
        SelectedRoom,
        GetActorLocation(),
        GetActorRotation(),
        SpawnParameters
    );

    // CurrentRoomがヌルの場合、失敗メッセージを表示する
    if (!CurrentRoom)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("Failed to spawn Room! (RoomArray%d[%d])"),
            CurrentDataIndex,
            CurrentDataArrayIndex
        );
        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("PlayerPos, (%.1f,%.1f,%.1f)"),
        Player->GetTargetLocation(Player).X,
        Player->GetTargetLocation(Player).Y,
        Player->GetTargetLocation(Player).Z
    );

    // プレイヤーを出発点まで移動させる
    Player->SetActorLocation(CurrentRoom->GetStartLocation());

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("PlayerPos, (%.1f,%.1f,%.1f)"),
        Player->GetTargetLocation(Player).X,
        Player->GetTargetLocation(Player).Y,
        Player->GetTargetLocation(Player).Z
    );
}

// 現在ルームを削除する
void ARoomManager::RemoveCurrentRoom()
{
    // 現在ルームがヌルの場合、処理しない
    if (!CurrentRoom)
        return;

    // 削除関数を呼び出す
    CurrentRoom->Destroy();

    CurrentRoom = nullptr;
}

