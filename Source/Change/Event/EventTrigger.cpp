// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTrigger.h"
#include "Components/BoxComponent.h"

// イベントを発送するためにインクルード
#include "Change/Event/GameEventSubsystem.h"

// コンストラクタ
AEventTrigger::AEventTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->SetupAttachment(RootComponent);
}

// ゲーム開始時、又は生成された時の処理
void AEventTrigger::BeginPlay()
{
	Super::BeginPlay();

	EventSystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

	if (BoxComponent)
	{
		// 接触時のイベント登録
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEventTrigger::OnOverlapBegin);
		// 離れ始めた時のイベント登録
		BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AEventTrigger::OnOverlapEnd);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}


// 接触し始めた時に呼ばれるイベント関数
UFUNCTION()
void AEventTrigger::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 相手はプレイヤじゃないと処理しない
	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	// イベントが設定されてない場合は処理しない
	if (TriggerEnterEvent == EGameEvent::None)
	{
		return;
	}

	EventSystem->SendEvent(TriggerEnterEvent, TriggerEnterMessage);
}

// 離れ始めた時に呼ばれるイベント関数
UFUNCTION()
void AEventTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	// 相手はプレイヤじゃないと処理しない
	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	// イベントが設定されてない場合は処理しない
	if (TriggerExitEvent == EGameEvent::None)
	{
		return;
	}

	EventSystem->SendEvent(TriggerExitEvent, TriggerExitMessage);
}