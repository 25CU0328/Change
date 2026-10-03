// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"	// IMC
#include "InputAction.h"			// IA
#include "InputActionValue.h"

#include "EPlayerState.h"			// プレイヤーの状態
#include "EPlayerDirection.h"		// プレイヤーの向き
#include "MainCharacter.generated.h"


// 入力関連クラスの前方宣言
class UIInputMappingContext;
class UInputAction;

UCLASS()
class CHANGE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AMainCharacter();
	// デストラクター
	~AMainCharacter();

protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

public:	
	// 毎フレーム呼ばれる処理
	virtual void Tick(float DeltaTime) override;

	// 入力コンポネントとメソッドのバインド
	virtual void SetupPlayerInputComponent(class UInputComponent* _playerInputComponent) override;

	// プレイヤーの移動処理
	void UpdateMovement(const FVector2f& MovementVector);
private:
	// プレイヤーの状態
	EPlayerState PlayerState;

	// プレイヤーの向き
	EPlayerDirection PlayerDirection;

	// 移動処理を行うためのコンポーネント
	UCharacterMovementComponent* MovementComponent;

private:
	// プレイヤーの向きを更新する
	void UpdatePlayerDirection();
	// プレイヤー状態を更新する
	void UpdatePlayerState();
};
