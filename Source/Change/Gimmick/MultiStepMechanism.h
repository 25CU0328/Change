// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechanismBase.h"
#include "MultiStepMechanism.generated.h"


// 前方宣言
class UBoxComponent;

/**
 * 
 */
UCLASS()
class CHANGE_API AMultiStepMechanism : public AMechanismBase
{
	GENERATED_BODY()
	
public:
	// コンストラクタ
	AMultiStepMechanism();

	// ゲーム開始時に呼ばれるメソッド
	void BeginPlay();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mechanism")
	int32 RequiredSteps;

	int32 TotalSteps = 0;
protected:
	// 活性化するイベントを受け取た時のコールバック
	virtual void OnActivateEvent(const FString& Message) override;
	// 非活性化するイベントを受け取た時のコールバック
	virtual void OnDeactiveEvent(const FString& Message) override;
};
