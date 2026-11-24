// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "TriggerableMechanism.generated.h"

/**
 * 
 */
UCLASS()
class CHANGE_API ATriggerableMechanism : public AFloorSwitch
{
	GENERATED_BODY()
public:
	// コンストラクタ
	ATriggerableMechanism();

	// ゲーム開始時に呼ばれる処理
	virtual void BeginPlay() override;

protected:
	// 衝突開始時呼び出された処理
	UFUNCTION()
	void OnBoxComponentOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

protected:
	TObjectPtr<UBoxComponent> BoxComponent;
};
