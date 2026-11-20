// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomArrayData.h"
#include "Change/Camera/StageCamera.h"
#include "RoomManager.generated.h"

// ルームのベースクラスの前方宣言
class ARoomBase;
class AMainCharacter;

// ルームを管理するマネージャー
UCLASS()
class CHANGE_API ARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクター
	ARoomManager();

	// プレイヤーを取得する
	AMainCharacter* GetPlayer();

	// 現在ルームを設定する
	void SetCurrentRoom(ARoomBase* NewRoom);
protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

public:
	// 最初のルームを設定するための変数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TObjectPtr<ARoomBase> InitialRoom;

private:
	// 現在ルーム
	TObjectPtr<ARoomBase> CurrentRoom;
	// プレイヤー
	TObjectPtr<AMainCharacter> Player;
	// ステージ内カメラ
	TObjectPtr<AStageCamera> StageCamera;
};
