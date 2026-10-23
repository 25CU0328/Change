#include "StageCamera.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"

#include "Change/Room/RoomBase.h"

// コンストラクタ
AStageCamera::AStageCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

    RootComponent = Camera;

    Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
    Camera->OrthoWidth = 300.0f;
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
}

void AStageCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateCameraPosition(DeltaTime);

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
        return;
    }

    

    // アスペクト比
    float AspectRatio = 16.0f / 9.0f;

    if (GEngine && GEngine->GameViewport)
    {
        FVector2D ViewportSize;

        GEngine->GameViewport->GetViewportSize(ViewportSize);

        if (ViewportSize.Y > 0.0f)
        {
            AspectRatio = ViewportSize.X / ViewportSize.Y;
        }
    }

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