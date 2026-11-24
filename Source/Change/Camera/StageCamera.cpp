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
    FadeInDuration = 1.0f;
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
    float NewCameraAspect = 16.0f / 9.0f; // (Bounds.Max.X - Bounds.Min.X)/ (Bounds.Max.Y - Bounds.Min.Y);
    UE_LOG(LogTemp, Warning, TEXT("StageCamera Aspect Ratio: %f"), NewCameraAspect);
    UE_LOG(LogTemp, Warning, TEXT("StageCamera Bounds: (%f, %f), (%f, %f)"), Bounds.Max.X, Bounds.Max.Y, Bounds.Min.X, Bounds.Min.Y);
    Camera->SetAspectRatio(NewCameraAspect);
    Camera->SetOrthoWidth((Bounds.Max.X - Bounds.Min.X));

    // 位置をタイルマップ中央に設定する
    FVector NewLocation = FVector(
        (Bounds.Max.X + Bounds.Min.X) / 2.0,
        (Bounds.Max.Y + Bounds.Min.Y) / 2.0,
        GetActorLocation().Z
    );

    SetActorLocation(NewLocation);

    UE_LOG(LogTemp, Warning, TEXT("StageCamera SetRoom"));
}

void AStageCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateCameraPosition(DeltaTime);

    UpdateFadeIn(0.0f);

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
		UE_LOG(LogTemp, Warning, TEXT("StageCamera: Target or CurrentRoom is not set."));
        return;
    }

    /*FVector DesiredLocation = Target->GetActorLocation();

    FVector NewLocation = FMath::VInterpTo(
        GetActorLocation(),
        DesiredLocation,
        DeltaTime,
        FollowSpeed
    );
    NewLocation = GetClampedPosition(NewLocation);

    SetActorLocation(NewLocation);*/

    SetActorLocation(GetClampedPosition(Target->GetActorLocation()));
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
    float AspectRatio = 16.0f / 9.0f; // (Bounds.Max.X - Bounds.Min.X) / (Bounds.Max.Y - Bounds.Min.Y);

    // 可視範囲の広さの半分
    float HalfViewportWidth = Camera->OrthoWidth * 0.5f;
    // 可視範囲の高さの半分
    float HalfViewporHeight = HalfViewportWidth / AspectRatio;


    const float MinX = Bounds.Min.X;
    const float MaxX = Bounds.Max.X;

    const float MinY = Bounds.Min.Y;
    const float MaxY = Bounds.Max.Y;

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

    // Y軸調整
    if (MaxY - MinY <= HalfViewporHeight * 2.0f)
    {
        ReturnVector.Y = (MinY + MaxY) * 0.5f;
    }
    else
    {
        ReturnVector.Y = FMath::Clamp(
            ReturnVector.Y,
            MinY + HalfViewporHeight,
            MaxY - HalfViewporHeight
        );
    }

	// Z軸はそのまま
	ReturnVector.Z = GetActorLocation().Z;

    return ReturnVector;
}

// フェードイン効果の更新処理
void AStageCamera::UpdateFadeIn(float Ratio)
{
   

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
    
    int32 ViewportWidth = 0;
    int32 ViewportHeight = 0;
    APawn* PlayerPawn = PlayerController->GetPawn();
    const FVector PlayerPos = PlayerPawn->GetActorLocation();
    
    FVector2D ScreenPosition;
    const bool bProjected = PlayerController->ProjectWorldLocationToScreen(
        PlayerPos,
        ScreenPosition
    );

    if (!bProjected)
    {
        return;
    }

    PlayerController->GetViewportSize(
        ViewportWidth,
        ViewportHeight
    );
    if (ViewportWidth <= 0 || ViewportHeight <= 0)
    {
        return;
    }

    
    const FVector2D PlayerUV(
        ScreenPosition.X / static_cast<float>(ViewportWidth),
        ScreenPosition.Y / static_cast<float>(ViewportHeight)
    );

    const float AspectRatio =
        16.0f / 9.0f;

	// 正投影カメラの幅を取得する
    const float OrthoWidth = Camera->OrthoWidth;

    FadeInMID->SetVectorParameterValue(
        TEXT("PlayerScreenPosition"),
        FLinearColor(
            PlayerUV.X,
            PlayerUV.Y,
            0.0f,
            0.0f
        )
    );

    // 
    FadeInMID->SetScalarParameterValue(
        TEXT("Gradient"),
        Gradient
    );

    FadeInMID->SetScalarParameterValue(
        TEXT("DisplayRange"),
        DisplayRange
    );

    FadeInMID->SetScalarParameterValue(
        TEXT("OrthoWidth"),
        OrthoWidth
    );

    FadeInMID->SetScalarParameterValue(
        TEXT("AspectRatio"),
        AspectRatio
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