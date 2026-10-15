// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h" 

// プレイヤーコントローラーのクラスを使うため
#include "Player/MainCharacterController.h"		

// PaperZDアニメションをコントロールするためインクルード
#include "PaperZDCharacter.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"

// UCharacterMovementComponent を使うためのヘッダー
#include "GameFramework/CharacterMovementComponent.h" 

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// デストラクター
AMainCharacter::~AMainCharacter()
{

}

// ゲーム開始時の処理
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // プレイヤーコントローラーにキャストできないなら処理しない
	auto* playerController = Cast<AMainCharacterController>(Controller);
    if (!playerController)
        return;

    // 入力を有効にする
    EnableInput(playerController);

	// 移動処理を行うためのコンポーネントを取得する
	MovementComponent = GetCharacterMovement();

	// アニメーションコンポネントを取得する
	AnimationComponent = FindComponentByClass<UPaperZDAnimationComponent>();

	// 変身データを管理するコンポネントを取得する
	CharacterFormDataComponent = FindComponentByClass<UCharacterFormDataComponent>();

	// プレイヤーの状態をアイドル状態に初期化
	CurrentPlayerState = EPlayerState::Idle;

	// プレイヤーの向きを右に初期化
	PlayerDirection = EPlayerDirection::Right;

	//　変身の状態を人間に初期化
	TransformState = ETransformState::Human;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 状態の更新
	UpdatePlayerState();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);
}

// プレイヤーの移動処理
void AMainCharacter::UpdateMovement(const FVector2f& MovementVector)
{
	// MovementComponentが取得できない場合は処理しない
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find MovementComponent!"));
		return;
	}

	if (MovementVector.X != 0.0f)
	{
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
		if (MovementComponent->Velocity.X != 0.0f)
		{
			// 移動を停止）
			MovementComponent->StopMovementImmediately();
		}

		// キャラクターの前方向に移動する
		AddMovementInput(FVector(0, 0, 1), MovementVector.Y);
	}
}


// プレイヤーの向きを更新する
void AMainCharacter::UpdateDirection(const FVector2f& MovementVector)
{
	// 水平方向の入力を優先する
	if (FMath::Abs(MovementVector.X) != 0.0f)
	{
		if (MovementVector.X >= 0.0f)
		{
			PlayerDirection = EPlayerDirection::Right;
		}
		else if (MovementVector.X < 0.0f)
		{
			PlayerDirection = EPlayerDirection::Left;
		}
	}
	else 
	{
		if (MovementVector.Y > 0.0f)
		{
			PlayerDirection = EPlayerDirection::Up;
		}
		else if (MovementVector.Y < 0.0f)
		{
			PlayerDirection = EPlayerDirection::Down;
		}
	}
}

// プレイヤー状態を更新する
void AMainCharacter::UpdatePlayerState()
{
	// 移動中かどうかで状態を更新する
	if (!FMath::IsNearlyZero(MovementComponent->Velocity.X) ||
		!FMath::IsNearlyZero(MovementComponent->Velocity.Z)
	)
	{
		CurrentPlayerState = EPlayerState::Moving;
	}
	else
	{
		CurrentPlayerState = EPlayerState::Idle;
	}
}

// 変身データを更新する
void AMainCharacter::ApplyCharacterFormData()
{
	// データを取得する
	auto Data = CharacterFormDataComponent->GetData(TransformState);
	// アニメーションインスタンスがヌルの場合、処理しない
	if (!Data.AnimationInstance)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Animation Instance Is Null!!")
		);
		return;
	}

	// アニメーションインスタンスを設定する
	AnimationComponent->SetAnimInstanceClass(Data.AnimationInstance);

	MovementComponent->BrakingDecelerationFlying = Data.Deceleration;
	MovementComponent->MaxFlySpeed = Data.MaxSpeed;
	MovementComponent->MaxAcceleration = Data.Acceleration;
}

// 猪に変身したときの処理
void AMainCharacter::UpdateTransformBoar()
{
	TransformState = ETransformState::Boar;

	// 変身後のデータを適用する
	ApplyCharacterFormData();
}

// 変身状態を解除するときの処理
void AMainCharacter::UpdateTransformCancel()
{
	TransformState = ETransformState::Human;

	// 変身後のデータを適用する
	ApplyCharacterFormData();
}
