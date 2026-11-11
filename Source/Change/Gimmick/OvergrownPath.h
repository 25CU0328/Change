// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "OvergrownPath.generated.h"

//前方宣言
class AMainCharacter;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class CHANGE_API AOvergrownPath : public AGimmickBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AOvergrownPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

private:
	// ギミックの能力
	void Gimmicke(float DeltaTime);

private:
	// プレイヤーが使用中か？
	bool bPlayerAbility;

	float Timer;

protected:
	// 衝突開始時呼び出される
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

	// 衝突終了時呼び出される
	virtual void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	)override;

protected:

	UPROPERTY()
	AMainCharacter* m_pPlayerChara;
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;

	// 接触用のコリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* pBoxComp;

	// ベースコンポ―ネット
	UPROPERTY()
	USceneComponent* pBase;

};
