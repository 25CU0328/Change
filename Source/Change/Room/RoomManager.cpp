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

	EventSubsystem->Register<ARoomManager>(
		EGameEvent::RoomReset,
		this,
		&ARoomManager::OnRoomReset
	);

    Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player not found."));
        return;
    }

    StageCamera = Cast<AStageCamera>(
        UGameplayStatics::GetActorOfClass(
            GetWorld(),
            AStageCamera::StaticClass()
        )
    );
    if (!StageCamera)
    {
        UE_LOG(LogTemp, Warning, TEXT("StageCamera not found."));
        return;
    }
    StageCamera->SetFollowTarget(Player);

    if (InitialRoom)
    {
        SetCurrentRoom(InitialRoom);
    }
}

// ルームリセット時に呼び出される関数
void ARoomManager::OnRoomReset(const FString& Message)
{
	if (CurrentRoom)
	{
		CurrentRoom->ResetRoom();
	}
}

// プレイヤーを取得する
AMainCharacter* ARoomManager::GetPlayer()
{
    return Player.Get();
}

// 現在ルームを設定する
void ARoomManager::SetCurrentRoom(ARoomBase* NewRoom)
{
    CurrentRoom = NewRoom;
    
	CurrentRoom->BeginPlay();

    auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
    EventSubsystem->SendEvent(EGameEvent::FadeInStart, "");

	StageCamera->SetRoom(CurrentRoom);
}
