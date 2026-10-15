// Fill out your copyright notice in the Description page of Project Settings.


#include "ARoomManager.h"
#include "ARoomBase.h"

#include "Change/Event/GameEventSubsystem.h"
// コンストラクター
AARoomManager::AARoomManager()
{
    CurrentRoom = nullptr;
    CurrentRoomIndex = 0;
}

// ゲーム開始時の処理
void AARoomManager::BeginPlay()
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
    EventSubsystem->Register<AARoomManager>(
        EGameEvent::RoomClear,
        this,
        &AARoomManager::OnRoomCleared
    );

    GenerateRoom();
}

// ルームがクリアされた時呼び出される関数
void AARoomManager::OnRoomCleared(const FString& EventData)
{
	// 現在ルームを削除
	RemoveCurrentRoom();

    // ルーム配列の番号を1増やす
	CurrentRoomIndex += 1;

    // 新しいルームを生成する
	GenerateRoom();
}

// ルームを生成する
void AARoomManager::GenerateRoom()
{
    // 現在ルームが存在する場合、削除する
    if (CurrentRoom)
    {
        RemoveCurrentRoom();
    }

	// 目標ルーム配列を表す変数
    const auto& RoomArrayData = RoomArrayDatas[CurrentRoomIndex];

    // 配列はヌルまたは空の場合、処理しない
    if (RoomArrayData.Rooms.IsEmpty())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Room%d array is empty!"),
            CurrentRoomIndex
        );
        return;
    }
    
    // ルーム配列内の番号を取得する
    const int RoomArrayIndex = FMath::RandRange(0, RoomArrayData.Rooms.Num() - 1);

    // 選択されたルームを取得する
    TSubclassOf<ARoomBase> SelectedRoom = RoomArrayData.Rooms[RoomArrayIndex];

    // 選択されたルームがヌルの場合、処理しない
    if (!SelectedRoom)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("RoomNuber %d, in RoomArray%d hasn't been set."),
            RoomArrayIndex,
            CurrentRoomIndex
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
            CurrentRoomIndex,
            RoomArrayIndex
        );
        return;
    }
}

// 現在ルームを削除する
void AARoomManager::RemoveCurrentRoom()
{
    // 現在ルームがヌルの場合、処理しない
    if (!CurrentRoom)
        return;

    // 削除関数を呼び出す
    CurrentRoom->Destroy();

    CurrentRoom = nullptr;
}

