// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h" 

// プレイヤーコントローラーのクラスを使うため
#include "Player/MainCharacterController.h"		
#include "Gimmick/Stone.h"

// PaperZDアニメションをコントロールするためインクルード
#include "PaperZDCharacter.h"
#include "PaperZDAnimationComponent.h"
#include "PaperZDAnimInstance.h"

// UCharacterMovementComponent を使うためのヘッダー
#include "GameFramework/CharacterMovementComponent.h" 
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// イベントを受けるためにインクルード
#include "Change/Event/GameEventSubsystem.h"

#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * ( -980.f) / 2 ) + ( m_jumpTime * 450.f )

// Sets default values
AMainCharacter::AMainCharacter()
	:m_bUesingAbility(false)
	, bIsPushingStone(false)
	, StopMove(false)
	, ForwardVector(0, 0, 0)
	, m_posBeforeJump(0, 0, 0)
	, m_jumpTime(0.f)
	, m_bJumping(false)
	, m_nowJumpHeight(0.f)
	, Start_Jump(0, 0, 0)
	, Controlpoint_Jump(0, 0, 0)
	, End_Jump(0, 0, 0)
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

	SpriteComponent = FindComponentByClass<UPaperSpriteComponent>();

	auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	EventManager->Register<AMainCharacter>(
		EGameEvent::RoomReset,
		this,
		&AMainCharacter::OnRoomReset
	);

	// 各状態の初期化
	InitPlayerStatus();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 状態の更新
	UpdatePlayerState();

	UpdateJump(DeltaTime);
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

	if (StopMove)
	{
		return;
	}

	if (MovementVector.X != 0.0f)
	{
		if (MovementComponent->Velocity.Y != 0.0f)
		{
			// 移動を停止）
			MovementComponent->StopMovementImmediately();
		}
		// キャラクターの右方向に移動する
		if (m_bUesingAbility)
		{
			if ((MovementVector.X != MovementConstraint.X ||
				MovementVector.Y != MovementConstraint.Y) &&
				MovementConstraint != FVector::ZeroVector
			)
			{

				UE_LOG(
					LogTemp,
					Warning,
					TEXT("MovementVector:(%.1f, %.1f), MovementConstraint:(%.1f, %.1f)"),
					MovementVector.X,
					MovementVector.Y,
					MovementConstraint.X,
					MovementConstraint.Y
				);
				return;
			}
				
		}


		AddMovementInput(FVector(1, 0, 0), MovementVector.X);
	}
	else if (MovementVector.Y != 0.0f)
	{
		if (MovementComponent->Velocity.X != 0.0f)
		{
			// 移動を停止）
			MovementComponent->StopMovementImmediately();
		}

		// キャラクターの右方向に移動する
		if (m_bUesingAbility)
		{
			if (m_bUesingAbility)
			{
				if ((MovementVector.X != MovementConstraint.X ||
					MovementVector.Y != MovementConstraint.Y ) &&
					MovementConstraint != FVector::ZeroVector
					)
				{

					UE_LOG(
						LogTemp,
						Warning,
						TEXT("MovementVector:(%.1f, %.1f), MovementConstraint:(%.1f, %.1f)"),
						MovementVector.X,
						MovementVector.Y,
						MovementConstraint.X,
						MovementConstraint.Y
					);
					return;
				}
			}
		}

		// キャラクターの前方向に移動する
		AddMovementInput(FVector(0, -1, 0), MovementVector.Y);
	}
}


// プレイヤーの向きを更新する
void AMainCharacter::UpdateDirection(const FVector2f& MovementVector)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateDirection!"));
	if (StopMove)
	{
		return;
	}

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

// 各プレイヤーステータスの初期化
void AMainCharacter::InitPlayerStatus()
{
	// プレイヤーの状態をアイドル状態に初期化
	CurrentPlayerState = EPlayerState::Idle;

	// プレイヤーの向きを右に初期化
	PlayerDirection = EPlayerDirection::Right;

	//　変身の状態を人間に初期化
	TransformState = ETransformState::Human;

	// 移動制限を初期化する
	MovementConstraint = FVector::ZeroVector;

	bIsPushingStone = false;

	m_bJumping = false;

	m_bUesingAbility = false;
}

// プレイヤー状態を更新する
void AMainCharacter::UpdatePlayerState()
{
	// 移動中かどうかで状態を更新する
	if (!FMath::IsNearlyZero(MovementComponent->Velocity.X) ||
		!FMath::IsNearlyZero(MovementComponent->Velocity.Y)
		)
	{
		CurrentPlayerState = EPlayerState::Move;
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
	if (m_bJumping)
		return;

	TransformState = ETransformState::Boar;

	// 変身後のデータを適用する
	ApplyCharacterFormData();
}

// 兎に変身したときの処理
void AMainCharacter::UpdateTransformRabbit()
{
	if (m_bJumping || bIsPushingStone)
		return;

	TransformState = ETransformState::Rabbit;

	// 変身後のデータを適用する
	ApplyCharacterFormData();
}

// 変身状態を解除するときの処理
void AMainCharacter::UpdateTransformCancel()
{
	if (m_bJumping || bIsPushingStone)
		return;

	TransformState = ETransformState::Human;

	// 変身後のデータを適用する
	ApplyCharacterFormData();
}

// 能力を使う時の処理
void AMainCharacter::UpdateUseAbility()
{
	if (TransformState == ETransformState::Rabbit)
	{
		UpdateRabbitUbility();
	}

	if (!bIsPushingStone) {
		//デバッグ確認用
		UE_LOG(LogTemp, Warning, TEXT("PLAYER not Ablity"));
		return;
	}

	if (!m_bUesingAbility)
	{
		m_bUesingAbility = true;
	}
	else
	{
		m_bUesingAbility = false;

	}
}

// 兎のアビリティ使用時の処理
void AMainCharacter::UpdateRabbitUbility()
{
	// ジャンプ中の場合は処理しない
	if (m_bJumping)
	{
		return;
	}

	// キャラクターの向きに対応する方向ベクトルを取得
	ForwardVector = SetForwardVector(PlayerDirection);

	// 現在位置を指定
	Start_Jump = GetActorLocation();
	// 現在位置とスプライトサイズと方向ベクトルを計算し着地点を計算
	End_Jump = GetActorLocation() + JumpDistance * ForwardVector;
	
	// ジャンプ先までの経路に障害物があるかどうかの確認
	// 障害物がある場合、は着地点を障害物の手前に設定する
	{
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		const float CheckRadius = 10.0f;
		bool bHit = GetWorld()->SweepSingleByChannel(
			Hit,
			Start_Jump,
			End_Jump,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(CheckRadius),
			Params
		);
		if (bHit)
		{
			// 着地点を障害物の手前に設定
			End_Jump = Hit.Location - ForwardVector * CheckRadius;
		}
	}

	// スタートとエンドからコントロールポイントを計算
	Controlpoint_Jump = FVector(
		(Start_Jump.X + FMath::Abs(End_Jump.Y - Start_Jump.Y) * 0.67f),
		(Start_Jump.Y - FMath::Abs(End_Jump.X - Start_Jump.X) * 0.67f),
		Start_Jump.Z + 2.0f
	) + 10.0f;

	// ジャンプフラグをtrueにする
	m_bJumping = true;
	Alpha = 0.f;
}

FVector AMainCharacter::SetForwardVector(EPlayerDirection _playerdirection)
{
	FVector vector(0, 0, 0);
	switch (_playerdirection)
	{
	case EPlayerDirection::Up:
		vector = FVector(0, -1, 0);
		break;
	case EPlayerDirection::Down:
		vector = FVector(0, 1, 0);
		break;
	case EPlayerDirection::Right:
		vector = FVector(1, 0, 0);
		break;
	case EPlayerDirection::Left:
		vector = FVector(-1, 0, 0);
		break;
	}

	return vector;
}

// ルームリセット時呼び出されるメソッド
void AMainCharacter::OnRoomReset(const FString& Message)
{
	// 各ステータスの初期化
	InitPlayerStatus();

	// 変身データを更新する
	ApplyCharacterFormData();
}

//ジャンプ処理
void AMainCharacter::UpdateJump(float Deltatime)
{
	if (m_bJumping)
	{
		//ジャンプ量を計算
		m_nowJumpHeight = JUMP_HEIGHT;

		Alpha += Deltatime;
		float T = FMath::Clamp(Alpha, 0.0f, 1.0f);
		FVector NewLocation = Jump(Start_Jump, Controlpoint_Jump, End_Jump, T);
		SetActorLocation(NewLocation);

		if (T == 1.f)
		{
			m_bJumping = false;
			Alpha = 0.f;
		}
	}
}

FVector AMainCharacter::Jump(FVector p1, FVector p2, FVector p3, float t)
{
	t = FMath::Clamp(t, 0.0f, 1.0f);

	FVector A = FMath::Lerp(p1, p2, t);
	FVector B = FMath::Lerp(p2, p3, t);
	return FMath::Lerp(A, B, t);
}

// 移動制限を設定する
void AMainCharacter::SetMovementConstraint(const FVector2f& NewConstraint)
{
	MovementConstraint = FVector(NewConstraint.X, NewConstraint.Y, 0.0f);
}