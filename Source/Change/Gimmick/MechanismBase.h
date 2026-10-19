// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Change/Event/EGameEvent.h"
#include "MechanismBase.generated.h"

class UPaperSpriteComponent;
class UPaperSprite;

UCLASS()
class CHANGE_API AMechanismBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMechanismBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	// 起動するためのイベント種類
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
	EGameEvent ActivateEvent;

	// 起動するためのイベントのメッセージ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
	FString ActivateMessage;

	// 解除するためのイベント種類
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
	EGameEvent DeactiveEvent;

	// 解除するためのイベントのメッセージ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event", meta = (AllowPrivateAccess = "true"))
	FString DeactivateMessage;

protected:
	// 起動イベントを受け取ったら
	void OnActivateEvent(const FString& Message);

	// 解除イベントを受け取ったら
	void OnDeactiveEvent(const FString& Message);
};
