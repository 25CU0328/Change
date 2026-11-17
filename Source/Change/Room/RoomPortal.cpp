// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPortal.h"
#include "Change/Event/GameEventSubsystem.h"

// コンストラクタ
ARoomPortal::ARoomPortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 衝突形状の作成
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(
		TEXT("BoxCollider")
	);

	TeleportPoint = CreateDefaultSubobject<USceneComponent>(
		TEXT("TeleportPoint")
	);
}

// ゲーム開始時または生成された時の処理
void ARoomPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// テレポートポイントを取得する
USceneComponent* ARoomPortal::GetTeleportPoint() const
{
	return TeleportPoint;
}

// 衝突開始時のコールバック
UFUNCTION()
void ARoomPortal::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 他のアクターはプレイヤーでなければ処理しない
	if (!OtherActor->ActorHasTag(FName("Player")))
	{
		return;
	}

	// テレポート先のポータルが設定されていなければ処理しない
	if (!TargetPortal)
	{
		return;
	}

	OtherActor->SetActorLocation(TargetPortal->GetTeleportPoint()->GetComponentLocation());
}
