// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomArrayData.h"
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
protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

public:
	// FRoomArrayDataを管理する配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<FRoomArrayData> RoomArrayDatas;

private:
	// ルーム番号を表す変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room", meta = (AllowPrivateAccess = "true"))
	int CurrentRoomIndex = 0;
private:
	// 現在ルーム
	TObjectPtr<ARoomBase> CurrentRoom;
	// プレイヤー
	TObjectPtr<AMainCharacter> Player;

	// --------------
	// コールバック関数
	// --------------
private:
	// ルームがクリアされた時呼び出される関数
	void OnRoomCleared(const FString& EventData);

	// ルームが全部クリアされたか
	bool IsAllRoomCleared();
private:
	// ルームを生成する
	void GenerateRoom();
	// 現在ルームを削除する
	void RemoveCurrentRoom();

};
