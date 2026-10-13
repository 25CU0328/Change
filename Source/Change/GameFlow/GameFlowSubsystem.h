// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFlowSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CHANGE_API UGameFlowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// サブシステムの初期化処理
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// サブシステムの終了処理
	virtual void Deinitialize() override;


private:
	// ゲーム開始に関するイベントが送られたら
	void OnGameStart(const FString& EventMessage);
	// ゲーム終了に関するイベントが送られたら
	void OnGameEnd(const FString& EventMessage);
};
