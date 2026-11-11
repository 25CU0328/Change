// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Change/Event/EGameEvent.h"
#include "Change/ETransformState.h"

#include "GimmickBase.generated.h"

class UPaperSpriteComponent;

UCLASS()
class CHANGE_API AGimmickBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGimmickBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;

protected:
	// 衝突開始時呼び出される
	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// 衝突終了時呼び出される
	UFUNCTION()
	virtual void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:
	// 対象の動物かどうかチェック
	virtual bool IsAnimalcheck(ETransformState etrasnformstate);

	// ギミックの能力
	virtual void Gimmick();

	// 能力使用しているか
	void IsUseAbility(bool useing);

protected:
	//能力を使用しているか
	bool IsUesAbility;

};
