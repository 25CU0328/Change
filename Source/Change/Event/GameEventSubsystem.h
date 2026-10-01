// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EGameEvent.h"
#include "GameEventSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(
	FGameEventDelegate,
	const FString&
);


// ゲーム内のイベントを管理するサブシステム
UCLASS()
class CHANGE_API UGameEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// サブシステムの初期化処理
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// サブシステムの終了処理
	virtual void Deinitialize() override;

public:
	template<typename T>
	void Register(EGameEvent Event, T* Object, void (T::* Callback)(const FString&))
	{
		// イベントが設定されていない場合は処理しない
		if (Event == EGameEvent::None)
			return;

		// コールバックをマップに追加する
		EventDelegates.FindOrAdd(Event).AddUObject(Object, Callback);
	}

	void SendEvent(EGameEvent Event, const FString& Message)
	{
		// Event種類に対応するコールバックを取得する
		auto Delegate = EventDelegates.Find(Event);
		// コールバックが設定されていない場合は処理しない
		if (!Delegate)
			return;

		// コールバックを順番に呼び出し、メッセージを渡す
		Delegate->Broadcast(Message);
	}

private:
	// イベントのコールバックを保持するマップ
	TMap<EGameEvent, FGameEventDelegate> EventDelegates;
};
