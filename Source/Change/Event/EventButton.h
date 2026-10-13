// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Change/Event/EGameEvent.h"
#include "EventButton.generated.h"

class UButton;
class UTextBlock;

// 押されたらイベントを送信するボタン
UCLASS()
class CHANGE_API UEventButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Designerで設定するボタンの表示文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText DisplayText;


protected:
	virtual void NativePreConstruct() override;

	// ボタン
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

	// 送信するイベントの種類
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Event")
	EGameEvent EventType;

	// イベントメッセージ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Event")
	FString EventMessage;

	// ボタンが押された時に呼び出される関数
	UFUNCTION()
	void OnButtonClicked();
};
