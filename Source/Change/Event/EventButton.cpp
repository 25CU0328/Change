// Fill out your copyright notice in the Description page of Project Settings.


#include "EventButton.h"

#include "Change/Event/GameEventSubsystem.h"

// ウィジェットコンポネント
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UEventButton::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (ButtonText)
    {
        // ボタンの表示テキストを設定する
        ButtonText->SetText(DisplayText);
    }

    if (Button)
    {
        // ボタンのクリックとコールバックのバインド
        Button->OnClicked.AddDynamic(
            this,
            &UEventButton::OnButtonClicked
        );
    }
}

// ボタンが押された時に呼び出される関数
UFUNCTION()
void UEventButton::OnButtonClicked()
{
    if (!GetGameInstance())
    {
        UE_LOG(LogTemp, Error, TEXT("Can't GetGameInstance!"));
        return;
    }

    UGameEventSubsystem* EventSubsystem =
        GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

    if (!EventSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't GetEventSubsystem!"));
    }

    EventSubsystem->SendEvent(EventType, EventMessage);
}