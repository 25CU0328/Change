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
	
	// --------------
	// レベル名
	// --------------
	const FName MainMenuName = "L_MainMenu";
	const FName StargeSceneName = "L_StageScene";
	const FName ClearSceneName = "L_ClearScene";

public:
	// サブシステムの初期化処理
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// サブシステムの終了処理
	virtual void Deinitialize() override;


private:
	// ゲーム開始に関するイベントが送られたら呼び出されるメソッド
	void OnGameStart(const FString& EventMessage);

	// ステージがクリアされたら呼び出されるメソッド
	void OnStageClear(const FString& EventMessage);

	// ゲーム終了に関するイベントが送られたら呼び出されるメソッド
	void OnGameEnd(const FString& EventMessage);

	// メインメニューに遷移するボタンをキリックしたら呼び出されるメソッド
	void OnToMainMenu(const FString& EventMessage);

};
