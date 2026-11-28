// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EGameEvent.h"
#include "EventTrigger.generated.h"

//前方宣言
class UBoxComponent;
class UGameEventSubsystem;

UCLASS()
class CHANGE_API AEventTrigger : public AActor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AEventTrigger();

protected:
	// ゲーム開始時、又は生成された時の処理
	virtual void BeginPlay() override;

	// 接触し始めた時に呼ばれるイベント関数
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 離れ始めた時に呼ばれるイベント関数
	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

protected:
	// イベントを管理するサブシステム
	TObjectPtr<UGameEventSubsystem> EventSystem;

	// ボックスコライダー
	TObjectPtr<UBoxComponent> BoxComponent;

protected:
	UPROPERTY(EditAnywhere, Category = "Event")
	EGameEvent TriggerEnterEvent;
	UPROPERTY(EditAnywhere, Category = "Event")
	FString TriggerEnterMessage;


	UPROPERTY(EditAnywhere, Category = "Event")
	EGameEvent TriggerExitEvent;
	UPROPERTY(EditAnywhere, Category = "Event")
	FString TriggerExitMessage;
};
