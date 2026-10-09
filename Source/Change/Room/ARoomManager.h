// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ARoomManager.generated.h"

// ルームのベースクラスの前方宣言
class ARoomBase;

// ルームを管理するマネージャー
UCLASS()
class CHANGE_API AARoomManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクター
	AARoomManager();

protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

public:
	// ルーム1の配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<TSubclassOf<ARoomBase>> Room1Array;

	// ルーム2の配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<TSubclassOf<ARoomBase>> Room2Array;

	// ルーム3の配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<TSubclassOf<ARoomBase>> Room3Array;

	// ルーム4の配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<TSubclassOf<ARoomBase>> Room4Array;

	// ルーム5の配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	TArray<TSubclassOf<ARoomBase>> Room5Array;

private:
	// ルーム番号を表す変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Room", meta = (AllowPrivateAccess = "true"))
	int CurrentRoomIndex = 0;
private:
	// 現在ルーム
	TObjectPtr<ARoomBase> CurrentRoom;

	// --------------
	// コールバック関数
	// --------------
private:
	// 現在ルームがクリアされたら呼び出す
	void OnCurrentRoomCleared();

private:
	// ルームを生成する
	void GenerateRoom();
	// 現在ルームを削除する
	void RemoveCurrentRoom();


};
