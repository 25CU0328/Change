// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"

#include "RoomStart.h"
#include "RoomGoal.h"
#include "PaperTileMapComponent.h"

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

protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

	// ゲーム終了時処理
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:

	// 出発点を取得する
	FVector GetStartLocation();
	
	// タイルマップのバウンディングを取得する
	FBox GetTilemapBounds() const;

protected:
	// ルームを管理するマネージャー
	UPROPERTY()
	TObjectPtr<ARoomManager> RoomManager;

	// ルームのタイルマップ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room")
	TObjectPtr<UPaperTileMapComponent> Tilemap;

	// ルームの出発点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anchor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> StartPointComponent;

	// ルームの終点
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anchor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UChildActorComponent> GoalPointComponent;
};
