// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "RoomBase.h"

#include "RoomPortal.generated.h"

// ルーム間移動用のポータル
UCLASS()
class CHANGE_API ARoomPortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクタ
	ARoomPortal();

	// テレポートポイントを取得する
	USceneComponent* GetTeleportPoint() const;

	ARoomBase* GetOwnerRoom() const;
protected:
	// ゲーム開始時または生成された時の処理
	virtual void BeginPlay() override;

	// 衝突開始時のコールバック
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	// 衝突形状
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	// このポータルのテレポート位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> TeleportPoint;

	// このポータルが属するルーム
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ARoomBase> OwnerRoom;

	// テレポート先のポータル
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ARoomPortal> TargetPortal;

	
};
