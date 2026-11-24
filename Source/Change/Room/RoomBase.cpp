// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomBase.h"
#include "RoomManager.h"
#include "Change/MainCharacter.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Change/Event/GameEventSubsystem.h"
// コンストラクタ
ARoomBase::ARoomBase()
{
}

// ゲーム開始時の処理
void ARoomBase::BeginPlay()
{
	if (bIsInitialized)
		return;

	Super::BeginPlay();

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	for (AActor* ChildActor : AttachedActors)
	{
		if (ChildActor)
		{
			// タイルマップコンポーネントを持つ子アクターを探す
			UPaperTileMapComponent* FoundComp = ChildActor->FindComponentByClass<UPaperTileMapComponent>();
			if (FoundComp)
			{
				Tilemap = FoundComp;
				break;
			}
		}
	}

	bIsInitialized = true;

	if (RespawnPointDummy) {
		PlayerRespawnPoint = RespawnPointDummy->GetActorLocation();
	}

	// イベントコールバックのバインド
	auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	EventManager->Register(
		EGameEvent::PlayerRespawn, 
		this, 
		&ARoomBase::OnPlayerRespawn
	);
}

// ゲーム終了時処理
void ARoomBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// プレイヤーがリスポーンしたときに呼ばれる関数
void ARoomBase::OnPlayerRespawn(const FString& Message)
{
	// プレイヤーを
	AActor* PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	PlayerActor->SetActorLocation(PlayerRespawnPoint);
}

// リスポーンポイントを設定する
void ARoomBase::SetRespawnPoint(const FVector& Location)
{
	PlayerRespawnPoint = Location;
}

// タイルマップのバウンディングを取得する
FBox ARoomBase::GetTilemapBounds() const
{
	return Tilemap->Bounds.GetBox();
}