// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h" // 追加
#include "Kismet/GameplayStatics.h" // 追加
#include "Components/InputComponent.h" // 追加
#include "EnhancedInputComponent.h" // 追加
#include "EnhancedInputSubsystems.h" // 追加

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
				&AMainCharacter::UpdateMovement
			);
		}

		// 変身のIAが設定された場合
		if (IA_Transform)
		{
			// ボタンが押された瞬間 (Started)
			EnhancedInputComponent->BindAction(
				IA_Transform, 
				ETriggerEvent::Started, 
				this, 
				&AMainCharacter::OnTransformStarted
			);

			// 長押しが完了した時 (Completed)
			EnhancedInputComponent->BindAction(
				IA_Transform, 
				ETriggerEvent::Completed, 
				this, 
				&AMainCharacter::OnTransformCompleted
			);

			// 長押しの途中で離された時 (Canceled)
			EnhancedInputComponent->BindAction(
				IA_Transform, 
				ETriggerEvent::Canceled, 
				this, 
				&AMainCharacter::OnTransformCanceled
			);
		}
	}
}

// プレイヤーの移動処理
void AMainCharacter::UpdateMovement(const FInputActionValue& InputValue)
{
	// 今は変身状態の場合、処理しない
	if (PlayerState == EPlayerState::Transform)
		return;

	// キーボードの入力を取得する
	FVector MovementVector = InputValue.Get<FVector>();
    UE_LOG(LogTemp, Log, TEXT("UpdateMovement"));
    PlayerState = EPlayerState::Move;

    if (Controller != nullptr)
    {
        // X 軸移動
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MovementVector.X);

        // Z 軸移動
        AddMovementInput(FVector(0.0f, 0.0f, 1.0f), MovementVector.Z);
    }
}


// 変形ボタンが押されたばかり
void AMainCharacter::OnTransformStarted(const FInputActionValue& InputValue)
{
    UE_LOG(LogTemp, Log, TEXT("Transform Start"));
    PlayerState = EPlayerState::Transform;
}
// 一定時間押されたら
void AMainCharacter::OnTransformCompleted(const FInputActionValue& InputValue)
{
    UE_LOG(LogTemp, Log, TEXT("Transform Complete"));
    PlayerState = EPlayerState::Idle;
}
// 途中で離されたら
void AMainCharacter::OnTransformCanceled(const FInputActionValue& InputValue)
{
    UE_LOG(LogTemp, Log, TEXT("Transform Canceled"));
    PlayerState = EPlayerState::Idle;
}