// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RoomGoal.generated.h"

UCLASS()
class CHANGE_API ARoomGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// コンストラクター
	ARoomGoal();

protected:
	// ゲーム開始時や生成された時の処理
	virtual void BeginPlay() override;

public:	

protected:
	// 衝突形状
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComponent;

	// 衝突開始時呼び出される
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

};
