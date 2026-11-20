// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"

#include "PaperTileMapComponent.h"
#include "PaperTileMapActor.h"

#include "RoomBase.generated.h"

class ARoomManager;

// シンプルな部屋ベースクラス
UCLASS()
class CHANGE_API ARoomBase : public AActor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	ARoomBase();

	// ゲーム開始時の処理
	virtual void BeginPlay() override;
protected:
	

	// ゲーム終了時処理
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// タイルマップのバウンディングを取得する
	FBox GetTilemapBounds() const;

protected:
	// ルームを管理するマネージャー
	UPROPERTY()
	TObjectPtr<ARoomManager> RoomManager;

	// ルームのタイルマップ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
	TObjectPtr<UPaperTileMapComponent> Tilemap;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Room")
	TObjectPtr<APaperTileMapActor> TilemapActor;

	// 初期化済みかどうかを示すフラグ
	bool bIsInitialized = false;
};
