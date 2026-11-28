// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiStepMechanism.h"
#include "PaperSPrite.h"
#include "PaperSpriteComponent.h"
// コンストラクタ
AMultiStepMechanism::AMultiStepMechanism()
{
}

// ゲーム開始時に呼ばれるメソッド
void AMultiStepMechanism::BeginPlay()
{
	Super::BeginPlay();
}

// 活性化するイベントを受け取た時のコールバック
void AMultiStepMechanism::OnActivateEvent(const FString& Message)
{
	if(Message != ActivateMessage)
	{
		return;
	}

	TotalSteps++;

	if (TotalSteps >= RequiredSteps)
	{
		ActiveMechanism();
	}
}

// 非活性化するイベントを受け取た時のコールバック
void AMultiStepMechanism::OnDeactiveEvent(const FString & Message)
{
	if(Message != DeactivateMessage)
	{
		return;
	}

	TotalSteps++;
	if (TotalSteps < RequiredSteps)
	{
		DeactiveMechanism();
	}
}
