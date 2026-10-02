// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加
#include "GameFramework/CharacterMovementComponent.h" // 追加：UCharacterMovementComponent を使うためのヘッダー

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // プレイヤーコントローラーにキャストできないなら処理しない
    APlayerController* playerController = Cast<APlayerController>(Controller);
    if (!playerController)
        return;

    // 入力を有効にする
    EnableInput(playerController);

    // Subsystemを取得できないなら処理しない
    UEnhancedInputLocalPlayerSubsystem* subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
    if (!subsystem)
        return;

	PlayerState = EPlayerState::Idle;

    // IMCが設定されたなら
    if (MappingContext) {
        // SubsystemにIMCを追加する
        subsystem->AddMappingContext(MappingContext, 0);
    }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(_playerInputComponent)) {
		// 移動のIAが設定された場合
		if (IA_PlayerMove) {
			// 移動処理と入力のバインド
			EnhancedInputComponent->BindAction(
				IA_PlayerMove,
				ETriggerEvent::Triggered,
				this,
				&AMainCharacter::OnMoveButtonPressed
			);
		}

		// 変身のIAが設定された場合
		if (IA_Transform_Boar)
		{
			// ボタンが押された瞬間 (Started)
			EnhancedInputComponent->BindAction(
				IA_Transform_Boar,
				ETriggerEvent::Started, 
				this, 
				&AMainCharacter::OnTransformBoarPressed
			);
		}

		// 兎に変身のIAが設定された場合
		if (IA_Transform_Rabbit)
		{
			// ボタンが押された瞬間 (Started)
			EnhancedInputComponent->BindAction(
				IA_Transform_Rabbit,
				ETriggerEvent::Started,
				this,
				&AMainCharacter::OnTransformRabbitPressed
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
				&AMainCharacter::OnUseAbilityPressed
			);
		}
	}
}

// プレイヤーの移動処理
void AMainCharacter::OnMoveButtonPressed(const FInputActionValue& InputValue)
{
	// 今は変身状態の場合、処理しない
	if (PlayerState == EPlayerState::Transform)
		return;

	// キーボードの入力を取得する
	FVector2f MovementVector = FVector2f(
		InputValue.Get<FVector>().X,
		InputValue.Get<FVector>().Z
	);

	
	if (MovementVector.X != 0.0f)
	{
		auto* MovementComponent = GetCharacterMovement();
		if (MovementComponent->Velocity.Z != 0.0f)
		{
			// 移動を停止）
			MovementComponent->StopMovementImmediately();
		}

		// キャラクターの右方向に移動する
		AddMovementInput(FVector(1, 0, 0), MovementVector.X);
		
	}
	else if (MovementVector.Y != 0.0f)
	{
		auto* MovementComponent = GetCharacterMovement();
		if (MovementComponent->Velocity.X != 0.0f)
		{
			// 移動を停止）
			MovementComponent->StopMovementImmediately();
		}

		// キャラクターの前方向に移動する
		AddMovementInput(FVector(0, 0, 1), MovementVector.Y);
	}
}

// 猪に変身するボタンが押されたら呼び出される関数
void AMainCharacter::OnTransformBoarPressed()
{

}

// 兎に変身するボタンが押されたら呼び出される関数
void AMainCharacter::OnTransformRabbitPressed()
{

}

// 能力使用ボタンが押されたら呼び出される関数
void AMainCharacter::OnUseAbilityPressed()
{

}