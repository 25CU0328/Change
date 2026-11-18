#include "StageCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"

#include "Change/Room/RoomBase.h"

// カメラのマテリアルを使用するためインクルード
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

// イベントを利用するためインクルード
#include "Change/Event/GameEventSubsystem.h"

// コンストラクタ
AStageCamera::AStageCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    RootComponent = Camera;

    Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
    Camera->OrthoWidth = 300.0f;

    FadeInCounter = 0.0f;
    FadeInDuration = 2.0f;
}

// ゲーム開始時または生成された時の処理
void AStageCamera::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController =
        GetWorld()->GetFirstPlayerController())
    {
        PlayerController->SetViewTarget(this);
    }

    auto* EventSubsystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
    if (!EventSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameEventSubsystem not available"));
        return;
    }

    // イベントを登録する
    EventSubsystem->Register<AStageCamera>(
        EGameEvent::RoomClear,
        this,
        &AStageCamera::OnRoomCleared
    );

    // イベントを登録する
    EventSubsystem->Register<AStageCamera>(
        EGameEvent::FadeInStart,
        this,
        &AStageCamera::OnFadeInStart
    );

    UpdateFadeIn(1.0f);
}

// ファローする対象を設定する
void AStageCamera::SetFollowTarget(AActor* NewTarget)
{
    Target = NewTarget;
}

// ルームを設定する
void AStageCamera::SetRoom(ARoomBase* NewRoom)
{
    CurrentRoom = NewRoom;

    // ルームのタイルマップのバウンド
    const FBox Bounds = CurrentRoom->GetTilemapBounds();
    float NewCameraAspect = (Bounds.Max.X - Bounds.Min.X)/ (Bounds.Max.Z - Bounds.Min.Z);

    Camera->SetAspectRatio(NewCameraAspect);
    Camera->SetOrthoWidth((Bounds.Max.X - Bounds.Min.X));

    // 位置をタイルマップ中央に設定する
    FVector NewLocation = FVector(
        (Bounds.Max.X + Bounds.Min.X) / 2.0,
        GetActorLocation().Y,
        (Bounds.Max.Z + Bounds.Min.Z) / 2.0
    );

    SetActorLocation(NewLocation);
}

void AStageCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateCameraPosition(DeltaTime);

    if (FadeInCounter > 0)
    {
        FadeInCounter -= DeltaTime;

        FadeInCounter = FMath::Clamp(FadeInCounter, 0.0f, FadeInDuration);

        UpdateFadeIn(1.0f - (FadeInCounter / FadeInDuration));

        if (FadeInCounter == 0)
        {
            auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

            EventManager->SendEvent(EGameEvent::FadeInEnd, "");
        }
    }
}

// カメラ位置の更新処理
void AStageCamera::UpdateCameraPosition(float DeltaTime)
{
    if (!Target || !CurrentRoom)
    {
        return;
    }

    FVector DesiredLocation = Target->GetActorLocation();

    FVector NewLocation = FMath::VInterpTo(
        GetActorLocation(),
        DesiredLocation,
        DeltaTime,
        FollowSpeed
    );
    NewLocation = GetClampedPosition(NewLocation);

    SetActorLocation(NewLocation);
}

// クランプされたカメラ位置を取得する
FVector AStageCamera::GetClampedPosition(const FVector& Location) const
{
    // 戻り値
    FVector ReturnVector = Location;

    if (!CurrentRoom || !Camera)
    {
        return ReturnVector;
    }

    // ルームのタイルマップのバウンド
    const FBox Bounds = CurrentRoom->GetTilemapBounds();

    if (!Bounds.IsValid)
    {
        return ReturnVector;
    }

    // アスペクト比
    float AspectRatio = (Bounds.Max.X - Bounds.Min.X) / (Bounds.Max.Z - Bounds.Min.Z);

    // 可視範囲の広さの半分
    float HalfViewportWidth = Camera->OrthoWidth * 0.5f;
    // 可視範囲の高さの半分
    float HalfViewporHeight = HalfViewportWidth / AspectRatio;


    const float MinX = Bounds.Min.X;
    const float MaxX = Bounds.Max.X;

    const float MinZ = Bounds.Min.Z;
    const float MaxZ = Bounds.Max.Z;

    // X軸調整
    if (MaxX - MinX <= HalfViewportWidth * 2.0f)
    {
        ReturnVector.X = (MinX + MaxX) * 0.5f;
    }
    else
    {
        ReturnVector.X = FMath::Clamp(
            ReturnVector.X,
            MinX + HalfViewportWidth,
            MaxX - HalfViewportWidth
        );
    }

    // Z軸調整
    if (MaxZ - MinZ <= HalfViewporHeight * 2.0f)
    {
        ReturnVector.Z = (MinZ + MaxZ) * 0.5f;
    }
    else
    {
        ReturnVector.Z = FMath::Clamp(
            ReturnVector.Z,
            MinZ + HalfViewporHeight,
            MaxZ - HalfViewporHeight
        );
    }

    return ReturnVector;
}

// フェードイン効果の更新処理
void AStageCamera::UpdateFadeIn(float Ratio)
{
    // マテリアル実体がないかのチェック
    if (!FadeInMID)
    {
        if (!FadeInMaterial)
        {
            UE_LOG(
                LogTemp,
                Warning,
                TEXT("StageCamera: RevealMaterial has not been assigned.")
            );
            return;
        }

        FadeInMID = UMaterialInstanceDynamic::Create(FadeInMaterial, this);

        if (!FadeInMID)
        {
            return;
        }

        Camera->AddOrUpdateBlendable(FadeInMID, 1.0f);
    }

    APlayerController* PlayerController =
        GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;

    if (!PlayerController)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("StageCamera: Cannot get PlayerController")
        );
        return;
    }

    // プレイヤー位置をスクリーン空間に投影する
    FVector2D ScreenPosition;
    if (!PlayerController->ProjectWorldLocationToScreen(
        Target->GetActorLocation(),
        ScreenPosition)
        )
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("StageCamera: Failed to ProjectWorldLocationToScreen")
        );
        return;
    }

    int32 ViewportWidth = 0;
    int32 ViewportHeight = 0;
    PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

    // 正規化した座標に転換する
    const float NormalizedScreenX = FMath::Clamp(
        ScreenPosition.X / static_cast<float>(ViewportWidth),
        0.0f,
        1.0f
    );

    const float NormalizedScreenY = FMath::Clamp(
        ScreenPosition.Y / static_cast<float>(ViewportHeight),
        0.0f,
        1.0f
    );

    // マテリアルインスタンスのScreenUVを設定する
    FadeInMID->SetVectorParameterValue(
        TEXT("ScreenUV"),
        FLinearColor(NormalizedScreenX, NormalizedScreenY, 0.0f, 0.0f)
    );

    // マテリアルインスタンスのRatio()を設定する
    FadeInMID->SetScalarParameterValue(
        TEXT("Ratio"),
        FMath::Clamp(Ratio, 0.0f, 1.0f)
    );

    // マテリアルインスタンスのAspect(アスペクト比)を設定する
    FadeInMID->SetScalarParameterValue(
        TEXT("Aspect"),
        static_cast<float>(ViewportWidth) /
        static_cast<float>(ViewportHeight)
    );
}


// ---------------------
// コールバック
// ---------------------

// ルームがクリアされた時呼び出される
void AStageCamera::OnRoomCleared(const FString& Message)
{
    UpdateFadeIn(0.0f);
}

// ルームが生成された時呼び出される
void AStageCamera::OnFadeInStart(const FString& Message)
{
    FadeInCounter = FadeInDuration;
}