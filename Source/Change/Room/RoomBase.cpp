// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomBase.h"
#include "RoomManager.h"
#include "Change/MainCharacter.h"
#include "Components/ChildActorComponent.h"


// コンストラクタ
ARoomBase::ARoomBase()
{
	StartPointComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("StartPointComponent"));
	StartPointComponent->SetupAttachment(GetRootComponent());

	GoalPointComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("GoalPointComponent"));
	GoalPointComponent->SetupAttachment(GetRootComponent());
}

// ゲーム開始時の処理
void ARoomBase::BeginPlay()
{
	Super::BeginPlay();

	Tilemap = GetComponentByClass<UPaperTileMapComponent>();
}

// ゲーム終了時処理
void ARoomBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// ルームマネージャーの設定
FVector ARoomBase::GetStartLocation()
{
	return StartPointComponent->GetComponentLocation();
}

// タイルマップのバウンディングを取得する
FBox ARoomBase::GetTilemapBounds() const
{
	return Tilemap->Bounds.GetBox();
}