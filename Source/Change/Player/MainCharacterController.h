// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"	// UInputMappingContextを使うため
#include "InputAction.h"			// UInputActionを使うため
#include "InputActionValue.h"		// FInputActionValueを使うため

#include "MainCharacterController.generated.h"


// 前方宣言
class AMainCharacter; // プレイヤーのクラス

/**
 * 
 */
UCLASS()
class CHANGE_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:
	// ゲーム開始時の処理
	virtual void BeginPlay() override;

public:
	// 入力とメソッドのバインド 
	virtual void SetupInputComponent() override;

private:
	// IAのマッピング
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;

	// 移動のIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_PlayerMove;

	// 猪に変形のするIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_TransformBoar;

	// 兎に変形のするIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_TransformRabbit;

	// 変身を解除のするIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_TransformCancel;

	// 能力使用のIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_UseAbility;

	AMainCharacter* MainCharacter; // プレイヤー
private:
	// プレイヤーの移動処理
	void OnIA_PlayerMove(const FInputActionValue& InputValue);
	// 猪に変身するボタンが押されたら呼び出される関数
	void OnIA_TransformBoar();
	// 兎に変身するボタンが押されたら呼び出される関数
	void OnIA_TransformRabbit();
	// 変身を解除するボタンが押されたら呼び出される関数
	void OnIA_TransformCancel();
	// 能力使用ボタンが押されたら呼び出される関数
	void OnIA_UseAbility();

};
