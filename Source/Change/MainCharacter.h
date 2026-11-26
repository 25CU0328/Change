// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EPlayerState.h"			// プレイヤーの状態
#include "EPlayerDirection.h"		// プレイヤーの向き
#include "ETransformState.h"		// 変身時の状態
#include "Gimmick/GimmickBase.h"
#include "Change/Player/CharacterFormDataComponent.h"
#include "MainCharacter.generated.h"

// アニメション関連クラス
class UPaperZDAnimInstance;
class UPaperZDAnimationComponent;
class UPaperSpriteComponent;

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

	// プレイヤーの向きを更新する
	void UpdateDirection(const FVector2f& MovementVector);

	// 猪に変身したときの処理
	void UpdateTransformBoar();

	// 兎に変身したときの処理
	void UpdateTransformRabbit();

	// 熊に変身したときの処理
	void UpdateTransformBear();

	// 変身状態を解除するときの処理
	void UpdateTransformCancel();

	// 能力を使用時の処理
	void UpdateUseAbility();

	// 能力が使用できるかチェック
	bool UseAbilityCheck();

	// 兎のアビリティ使用時の処理
	void UpdateRabbitUbility();

	// ジャンプ処理
	void UpdateJump(float Deltatime);

	// ダッシュ使用時の処理
	void UpdateUseDash();

	// ダッシュ中の処理
	void UpdateDash(float Deltatime);

	// 突撃中の処理
	void UpdateCharge();

	FVector Jump(FVector p1, FVector p2, FVector p3, float t);

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

	// 兎のアニメションインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TSubclassOf<UPaperZDAnimInstance> AS_Ribbit;


	// プレイヤーの状態
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerState CurrentPlayerState;

	// プレイヤーの向き
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerDirection PlayerDirection;

	// 変身時の状態
	ETransformState TransformState;
private:
	// 移動処理を行うためのコンポーネント
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	// 変身データを管理するコンポネント
	TObjectPtr<UCharacterFormDataComponent> CharacterFormDataComponent;

	UPROPERTY()
	AGimmickBase* m_pLiftObject;					//持ち上げているオブジェクト情報
	UPROPERTY()
	float Alpha;

public:
	// 現在の変身状態を返す
	ETransformState GetTransformState() { return TransformState; }

	// 現在の変身状態を返す
	EPlayerState GetPlayertate() { return PlayerState; }

	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;

	// 移動制限を設定する
	void SetMovementConstraint(const FVector2f& NewConstraint);

	void SetPushingStone(bool b_flg) { bIsPushingStone = b_flg; }

	bool GetIsUesAbility() { return m_bUesingAbility; }

	void SetMove(bool _floa) { 
		_floa = StopMove;
		UE_LOG(LogTemp, Warning, TEXT("Stop"));
	}
	bool GetMove() { return StopMove; }

private:
	
	// プレイヤー状態を更新する
	void UpdatePlayerState();

	// 各プレイヤーステータスの初期化
	void InitPlayerStatus();

	// 変身データを適用する
	void ApplyCharacterFormData();
	// アビリティを使用時のレイキャスト処理
	void UpdateUesAbilityLine();

	FVector SetForwardVector(EPlayerDirection _playerdirection);

	// ルームリセット時呼び出されるメソッド
	void OnRoomReset(const FString& Message);
private:
	//アビリティ使用状態フラグ

	bool m_bUesingAbility;
	bool bIsPushingStone;
	bool StopMove;

	EPlayerState PlayerState;

	// 移動制限
	FVector MovementConstraint;

	FVector ForwardVector;
	FVector m_posBeforeJump;
	float m_jumpTime;
	bool m_bJumping;
	float m_nowJumpHeight;
	FVector Start_Jump;
	FVector Controlpoint_Jump;
	FVector End_Jump;

private:
	// 能力使用に関係する諸々
	// 猪関連
	float DashGauge;		// ダッシュゲージ
	bool bDashing;			// ダッシュ中か？
	float DashTime;			// ダッシュ時間
	float CurrentSpeed;		// 現在の速度
	float MaxSpeed;			// 最高速度
	bool bChargeing;		// 突撃中か？
	UPROPERTY()
	float ChargeThreshold;	// 突撃に入る閾値

	// 象関連
	

};
