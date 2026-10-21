// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EPlayerState.h"			// プレイヤーの状態
#include "EPlayerDirection.h"		// プレイヤーの向き
#include "ETransformState.h"		// 変身時の状態
#include "Change/Player/CharacterFormDataComponent.h"
#include "MainCharacter.generated.h"

// アニメション関連クラス
class UPaperZDAnimInstance;
class UPaperZDAnimationComponent;

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

	// 猪に変身したときの処理
	void UpdateTransformBoar();

	// 変身状態を解除するときの処理
	void UpdateTransformCancel();

	void UpdateUseAbility();

	// プレイヤーの向きを更新する
	void UpdateDirection(const FVector2f& MovementVector);


public:
	// アニメションをコントロールするコンポネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperZDAnimationComponent> AnimationComponent;

	// 人類のアニメションインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UPaperZDAnimInstance> AS_Human;

	// 猪のアニメションインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UPaperZDAnimInstance> AS_Boar;

private:
	// プレイヤーの状態
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerState CurrentPlayerState;

	// プレイヤーの向き
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerDirection PlayerDirection;

	// 変身時の状態
	ETransformState TransformState;

	// 移動処理を行うためのコンポーネント
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	// 変身データを管理するコンポネント
	TObjectPtr<UCharacterFormDataComponent> CharacterFormDataComponent;

	UPROPERTY()
	AActor* m_pLiftObject;					//持ち上げているオブジェクト情報

public:
	// 現在の変身状態を返す
	ETransformState GetTransformState() { return TransformState; }

	// 現在の変身状態を返す
	EPlayerState GetPlayertate() { return PlayerState; }


	void SetCanuesAbility(bool b_flg) { CanuesAbility = b_flg; }

	bool GetIsUesAbility() { return m_bUesingAbility; }

	void SetMove(bool _floa) { _floa = StopMove; }
	bool GetMove() { return StopMove; }
private:
	
	// プレイヤー状態を更新する
	void UpdatePlayerState();

	// 変身データを適用する
	void ApplyCharacterFormData();
	// アビリティを使用時のレイキャスト処理
	void UpdateUesAbilityLine();

private:
	//アビリティ使用状態フラグ

	bool m_bUesingAbility;
	bool CanuesAbility;
	bool StopMove;

	EPlayerState PlayerState;
};
