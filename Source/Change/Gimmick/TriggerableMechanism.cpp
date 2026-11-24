// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerableMechanism.h"
#include "PaperSPrite.h"
#include "PaperSpriteComponent.h"
#include "Change/Event/GameEventSubsystem.h"

// コンストラクタ
ATriggerableMechanism::ATriggerableMechanism()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(
		TEXT("BoxComponent")
	);
}

// ゲーム開始時に呼ばれる処理
void ATriggerableMechanism::BeginPlay()
{
	// Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&ATriggerableMechanism::OnBoxComponentOverlap
	);
}

// 衝突開始時呼び出された処理
void ATriggerableMechanism::OnBoxComponentOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	// 対象はPlayerじゃない場合、処理しない
	if (!OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("ATriggerableMechanism::OnBoxComponentOverlap: Player has overlapped with the triggerable mechanism.")
	);

	// イベント発送
	auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

	EventManager->SendEvent(PressedEvent, PressedEventMessage);
	
}