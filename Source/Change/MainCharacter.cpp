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
    if (m_mappingContext) {
        // SubsystemにIMCを追加する
        subsystem->AddMappingContext(m_mappingContext, 0);
    }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_pPlayerInputComponent);

    if (UEnhancedInputComponent* pEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(_pPlayerInputComponent)) {

        // 移動処理と入力のバインド
        pEnhancedInputComponent->BindAction(
            m_IA_playerMove, 
            ETriggerEvent::Triggered,
            this,
            &AMainCharacter::UpdateMovement
        );
    }
}

// プレイヤーの移動処理
void AMainCharacter::UpdateMovement(const FInputActionValue& _value)
{
	// キーボードの入力を取得する
	FVector MovementVector = _value.Get<FVector>();

    if (Controller != nullptr)
    {
        // X 軸移動
        AddMovementInput(FVector(1.0f, 0.0f, 0.0f), MovementVector.X);

        // Z 軸移動
        AddMovementInput(FVector(0.0f, 0.0f, 1.0f), MovementVector.Z);
    }
}


// 変形ボタンが押されたばかり
void AMainCharacter::OnTransformStarted(const FInputActionValue& _value)
{
    UE_LOG(LogTemp, Log, TEXT("Transform Start"));
    // 変身開始アニメションやエフェクト
}
// 一定時間押されたら
void AMainCharacter::OnTransformCompleted(const FInputActionValue& _value)
{
    UE_LOG(LogTemp, Log, TEXT("Transform Complete"));
    // 変身アニメション
}
// 途中で離されたら
void AMainCharacter::OnTransformCanceled(const FInputActionValue& _value)
{
    UE_LOG(LogTemp, Log, TEXT("Transform Canceled"));
    // 変身諦めアニメションやエフェクト
}