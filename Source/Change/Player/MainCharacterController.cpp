// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "EnhancedInputComponent.h"	// UEnhancedInputComponentを使うため
#include "EnhancedInputSubsystems.h"// UEnhancedInputLocalPlayerSubsystemを使うため

#include "Change/MainCharacter.h" // プレイヤーのクラスをアクセスするため

// ゲーム開始時の処理
void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

    // Subsystemを取得できないなら処理しない
    auto* subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!subsystem)
        return;

    // IMCが設定されたなら
    if (InputMappingContext) {
        // SubsystemにIMCを追加する
        subsystem->AddMappingContext(InputMappingContext, 0);
    }

	MainCharacter = Cast<AMainCharacter>(GetPawn());
}

// 入力とメソッドのバインド
void AMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// UEnhancedInputComponentを取得する
	auto* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	// 失敗した場合は処理しない
	if (!EnhancedInputComponent)
		return;

	// 移動のIAが設定された場合
	if (IA_PlayerMove) {
		// 移動処理と入力のバインド
		EnhancedInputComponent->BindAction(
			IA_PlayerMove,
			ETriggerEvent::Triggered,
			this,
			&AMainCharacterController::OnIA_PlayerMove
		);
	}

	// 変身のIAが設定された場合
	if (IA_TransformBoar)
	{
		// ボタンが押された瞬間 (Started)
		EnhancedInputComponent->BindAction(
			IA_TransformBoar,
			ETriggerEvent::Started,
			this,
			&AMainCharacterController::OnIA_TransformBoar
		);
	}

	// 兎に変身のIAが設定された場合
	if (IA_TransformRabbit)
	{
		// ボタンが押された瞬間 (Started)
		EnhancedInputComponent->BindAction(
			IA_TransformRabbit,
			ETriggerEvent::Started,
			this,
			&AMainCharacterController::OnIA_TransformRabbit
		);
	}

	// 変身の解除のIAが設定された場合
	if (IA_TransformCancel)
	{
		// ボタンが押された瞬間 (Started)
		EnhancedInputComponent->BindAction(
			IA_TransformCancel,
			ETriggerEvent::Started,
			this,
			&AMainCharacterController::OnIA_TransformCancel
		);
	}

	// 能力使用のIAが設定された場合
	if (IA_UseAbility)
	{
		// ボタンが押された瞬間 (Started)
		EnhancedInputComponent->BindAction(
			IA_UseAbility,
			ETriggerEvent::Started,
			this,
			&AMainCharacterController::OnIA_UseAbility
		);
	}
}



// プレイヤーの移動処理
void AMainCharacterController::OnIA_PlayerMove(const FInputActionValue& InputValue)
{
	// キーボードの入力から移動ベクトルを取得
	FVector2f MovementVector = FVector2f(
		InputValue.Get<FVector>().X,
		InputValue.Get<FVector>().Z
	);

	MainCharacter->UpdateMovement(MovementVector);
}

// 猪に変身するボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_TransformBoar()
{
	MainCharacter->UpdateTransformBoar();
}

// 兎に変身するボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_TransformRabbit()
{

}

// 変身を解除するボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_TransformCancel()
{
	MainCharacter->UpdateTransformCancel();
}

// 能力使用ボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_UseAbility()
{

}