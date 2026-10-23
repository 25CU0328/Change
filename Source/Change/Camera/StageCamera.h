#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageCamera.generated.h"

class UCameraComponent;
class ARoomBase;

UCLASS()
class CHANGE_API AStageCamera : public AActor
{
    GENERATED_BODY()

public:
    // コンストラクタ
    AStageCamera();

    // ファローする対象を設定する
    void SetFollowTarget(AActor* NewTarget);

    // ルームを設定する
    void SetRoom(ARoomBase* NewRoom);
public:
    // 毎フレームの更新処理
    virtual void Tick(float DeltaTime) override;

protected:
    // ゲーム開始時または生成された時の処理
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Camera")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, Category = "Camera")
    float FollowSpeed = 5.0f;

    UPROPERTY()
    TObjectPtr<AActor> Target;

    UPROPERTY()
    TObjectPtr<ARoomBase> CurrentRoom;

protected:
    // カメラ位置の更新処理
    void UpdateCameraPosition(float DeltaTime);
    // クランプされたカメラ位置を取得する
    FVector GetClampedPosition(const FVector& Location) const;


};