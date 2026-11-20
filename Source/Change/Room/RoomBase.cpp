// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomBase.h"
#include "RoomManager.h"
#include "Change/MainCharacter.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"

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
			// 從子 Actor 身上尋找 PaperTileMapComponent
			UPaperTileMapComponent* FoundComp = ChildActor->FindComponentByClass<UPaperTileMapComponent>();
			if (FoundComp)
			{
				Tilemap = FoundComp;
				break; // 找到第一個符合的就停止搜尋
			}
		}
	}

	bIsInitialized = true;
}

// ゲーム終了時処理
void ARoomBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// タイルマップのバウンディングを取得する
FBox ARoomBase::GetTilemapBounds() const
{
	return Tilemap->Bounds.GetBox();
}