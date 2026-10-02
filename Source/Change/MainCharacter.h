// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"	// IMC
#include "InputAction.h"			// IA
#include "InputActionValue.h"		// 追加

#include "EPlayerState.h"			// プレイヤーの状態
#include "MainCharacter.generated.h"



// 入力関連クラスの前方宣言
class UIInputMappingContext;
class UInputAction;

UCLASS()
class CHANGE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _playerInputComponent) override;

	// 入力関連
private:
	EPlayerState PlayerState;

private:
	// IAのマッピング
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;

	// 移動のIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_PlayerMove;

	// 猪に変形のするIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Transform_Boar;

	// 兎に変形のするIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Transform_Rabbit;


	// 能力使用のIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_UseAbility;

private:
	// プレイヤーの移動処理
	void OnMoveButtonPressed(const FInputActionValue& InputValue);
	// 猪に変身するボタンが押されたら呼び出される関数
	void OnTransformBoarPressed();
	// 兎に変身するボタンが押されたら呼び出される関数
	void OnTransformRabbitPressed();
	// 能力使用ボタンが押されたら呼び出される関数
	void OnUseAbilityPressed();
};
