#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageCamera.generated.h"

class UCameraComponent;
class ARoomBase;
class UMaterialInterface;
class UMaterialInstanceDynamic;

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
    // マテリアル
    UPROPERTY(EditAnywhere, Category = "FadeIn")
    TObjectPtr<UMaterialInterface> FadeInMaterial;

    // 実行時のマテリアルインスタンス
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> FadeInMID;

    // フェードイン時間
    UPROPERTY(EditAnywhere, Category = "FadeIn")
    float FadeInDuration;

    // フェードインを更新するためのカウンター
    float FadeInCounter;

protected:
    // フェードイン効果の更新処理
    UFUNCTION()
    void UpdateFadeIn(float Ratio);

    // カメラ位置の更新処理
    void UpdateCameraPosition(float DeltaTime);
    // クランプされたカメラ位置を取得する
    FVector GetClampedPosition(const FVector& Location) const;

    // コールバック
protected:
    // ルームがクリアされた時呼び出される
    void OnRoomCleared(const FString& Message);

    // ルームが生成された時呼び出される
    void OnRoomCreated(const FString& Message);
};