// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterController.h"
#include "EnhancedInputComponent.h"	// UEnhancedInputComponentを使うため
#include "EnhancedInputSubsystems.h"// UEnhancedInputLocalPlayerSubsystemを使うため

#include "Change/Event/GameEventSubsystem.h" // イベントマネージャーを取得するため

#include "Change/MainCharacter.h" // プレイヤーのクラスをアクセスするため
#include "Change/EPlayerState.h"

#include "Change/Event/GameEventSubsystem.h"

// ゲーム開始時の処理
void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	MainCharacter = Cast<AMainCharacter>(GetPawn());

	bEnableInput = true;

	auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (!EventManager)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("[AMainCharacterController]: EventManger is Null")
		);
		return;
	}

	EventManager->Register<AMainCharacterController>(
		EGameEvent::FadeInStart,
		this,
		&AMainCharacterController::OnFadeInStart
	);

	EventManager->Register<AMainCharacterController>(
		EGameEvent::FadeInEnd,
		this,
		&AMainCharacterController::OnFadeInEnd
	);
}

// 入力とメソッドのバインド
void AMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// UEnhancedInputComponentを取得する
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	// 失敗した場合は処理しない
	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast Enhance Input Component!!"));
		return;
	}

	// Subsystemを取得できないなら処理しない
	auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to GetSubsystem!!"));
		return;
	}

	// IMCが設定されたなら
	if (InputMappingContext) {
		// SubsystemにIMCを追加する
		subsystem->AddMappingContext(InputMappingContext, 0);
	}


	InputComponent->BindKey(EKeys::G, IE_Pressed, this, &AMainCharacterController::OnIA_TransformBoar);

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
		// ボタンが押された瞬間
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
	// 入力を受け付けないと設定する場合、処理しない
	if (!bEnableInput)
		return;

	// キーボードの入力から移動ベクトルを取得
	FVector2f MovementVector = FVector2f(
		InputValue.Get<FVector>().X,
		InputValue.Get<FVector>().Y
	);

	// 移動の更新処理
	MainCharacter->UpdateMovement(MovementVector);

	// 向きの更新処理
	if (!MainCharacter->GetIsUesAbility())
	{
	    MainCharacter->UpdateDirection(MovementVector);
	}
}

// 猪に変身するボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_TransformBoar()
{
	// 入力を受け付けないと設定する場合、処理しない
	if (!bEnableInput)
		return;

	MainCharacter->UpdateTransformBoar();
}

// 兎に変身するボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_TransformRabbit()
{
	// 入力を受け付けないと設定する場合、処理しない
	if (!bEnableInput)
		return;

	MainCharacter->UpdateTransformRabbit();
}

// 変身を解除するボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_TransformCancel()
{
	// 入力を受け付けないと設定する場合、処理しない
	if (!bEnableInput)
		return;

	MainCharacter->UpdateTransformCancel();
}

// 能力使用ボタンが押されたら呼び出される関数
void AMainCharacterController::OnIA_UseAbility()
{	
	// 入力を受け付けないと設定する場合、処理しない
	if (!bEnableInput)
		return;

	//UGameEventSubsystem* pEventSubsystem = Cast<UGameEventSubsystem>(GetGameInstance());
	//pEventSubsystem->SendEvent(EGameEvent::UseAbility, "UseAbility");
	MainCharacter->UpdateUseAbility();
}

// フェードイン開始時呼び出されるメソッド
void AMainCharacterController::OnFadeInStart(const FString& Message)
{
	bEnableInput = false;

	// プレイヤーの移動を止まる
	MainCharacter->GetMovementComponent()->StopMovementImmediately();
}

// フェードイン終了時時呼び出されるメソッド
void AMainCharacterController::OnFadeInEnd(const FString& Message)
{
	bEnableInput = true;
}