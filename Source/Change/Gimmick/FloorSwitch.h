// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Change/Event/EGameEvent.h"

#include "FloorSwitch.generated.h"

class UPaperSpriteComponent;
class UPaperSprite;

UCLASS()
class CHANGE_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
    UPROPERTY()
    TObjectPtr<UPaperSpriteComponent> SpriteComponent;

    // 元状態の画像
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPaperSprite> NormalSprite;
    // 元状態のカラー
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    FLinearColor NormalColor = FLinearColor::White;

    // 押された時の画像
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPaperSprite> PressedSprite;
    // 押された時のカラー
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
    FLinearColor PressedColor = FLinearColor::White;
protected:
    // 送信するイベント種類
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
    EGameEvent PressedEvent;

    // 送信するイベントのメッセージ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
    FString PressedEventMessage;

    // 送信するイベント種類
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
    EGameEvent ReleaseEvent;

    // 送信するイベントのメッセージ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
    FString ReleaseEventMessage;
   
protected:
    // 衝突開始時呼び出される
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    // 衝突終了時呼び出される
    UFUNCTION()
    void OnOverlapEnd(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );
};
