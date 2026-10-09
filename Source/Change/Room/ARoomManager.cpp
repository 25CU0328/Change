// Fill out your copyright notice in the Description page of Project Settings.


#include "ARoomManager.h"
#include "ARoomBase.h"
// コンストラクター
AARoomManager::AARoomManager()
{

}

// ゲーム開始時の処理
void AARoomManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// 現在ルームがクリアされたら呼び出す
void AARoomManager::OnCurrentRoomCleared()
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
	TArray<TSubclassOf<ARoomBase>>* RoomArray = nullptr;

    switch (CurrentRoomIndex)
    {
    case 0:
        RoomArray = &Room1Array;
        break;

    case 1:
        RoomArray = &Room2Array;
        break;

    case 2:
        RoomArray = &Room3Array;
        break;

    case 3:
        RoomArray = &Room4Array;
        break;

    case 4:
        RoomArray = &Room5Array;
        break;

    default:
        // ルーム 1~5全部完成した場合
        return;
    }

    // 配列はヌルまたは空の場合、処理しない
    if (RoomArray == nullptr || RoomArray->IsEmpty())
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
    const int RoomArrayIndex = FMath::RandRange(0, RoomArray->Num() - 1);

    // 選択されたルームを取得する
    TSubclassOf<ARoomBase> SelectedRoom = (*RoomArray)[RoomArrayIndex];

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

