// Fill out your copyright notice in the Description page of Project Settings.


#include "MechanismBase.h"
#include "PaperSPrite.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

#include "Change/Event/GameEventSubsystem.h"
// Sets default values
AMechanismBase::AMechanismBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMechanismBase::BeginPlay()
{
	Super::BeginPlay();
	
	auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	if (EventManager)
	{
		if (ActivateEvent != EGameEvent::None)
		{
			EventManager->Register<AMechanismBase>(
				ActivateEvent,
				this,
				&AMechanismBase::OnActivateEvent
			);
		}

		if (DeactiveEvent != EGameEvent::None)
		{
			EventManager->Register<AMechanismBase>(
				DeactiveEvent,
				this,
				&AMechanismBase::OnDeactiveEvent
			);
		}
	}

	// スプライトコンポネントを取得する
	SpriteComponent = GetComponentByClass<UPaperSpriteComponent>();

	// 表示を変更
	if (NormalSprite) {
		SpriteComponent->SetSprite(NormalSprite);
		SpriteComponent->SetSpriteColor(NormalColor);
	}

	BoxComponent = GetComponentByClass<UBoxComponent>();
}

// 起動イベントを受け取ったら
void AMechanismBase::OnActivateEvent(const FString& Message)
{
	// 起動メッセージが設定されかつイベントメッセージと異なる場合、処理しない
	if (ActivateMessage != Message && !ActivateMessage.IsEmpty())
	{
		return;
	}

	ActiveMechanism();
}

// 解除イベントを受け取ったら
void AMechanismBase::OnDeactiveEvent(const FString& Message)
{
	// 解除メッセージが設定されかつイベントメッセージと異なる場合、処理しない
	if (DeactivateMessage != Message && !DeactivateMessage.IsEmpty())
	{
		return;
	}

	DeactiveMechanism();
}

// ギミックを起動する
void AMechanismBase::ActiveMechanism()
{
	if (PressedSprite) {
		SpriteComponent->SetSprite(PressedSprite);
		SpriteComponent->SetSpriteColor(PressedColor);
	}

	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// ギミックを解除する
void AMechanismBase::DeactiveMechanism()
{
	if (NormalSprite) {
		SpriteComponent->SetSprite(NormalSprite);
		SpriteComponent->SetSpriteColor(NormalColor);
	}

	BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}