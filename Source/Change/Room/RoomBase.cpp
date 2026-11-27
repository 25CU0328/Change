// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomBase.h"
#include "RoomManager.h"
#include "Change/MainCharacter.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Change/Event/GameEventSubsystem.h"
// コンストラクタ
ARoomBase::ARoomBase()
{
}

// ゲーム開始時の処理
void ARoomBase::BeginPlay()
{
	if (bIsInitialized)
		return;

	Super::BeginPlay();

	TArray<UChildActorComponent*> AttachedChildActors;
	GetComponents<UChildActorComponent>(
		AttachedChildActors
	);

	for (UChildActorComponent* ChildComponent : AttachedChildActors)
	{
		if (ChildComponent)
		{
			AActor* ChildActor = ChildComponent->GetChildActor();

			if (ChildActor)
			{
				FVector Location = ChildActor->GetActorLocation();
				ChildInitPositions.Add(
					ChildComponent,
					ChildComponent->GetChildActor()->GetActorLocation()
				);
			}
		}
	}

	Tilemap = TilemapActor->GetRenderComponent();

	bIsInitialized = true;

	if (RespawnPointDummy) {
		PlayerRespawnPoint = RespawnPointDummy->GetActorLocation();
	}

	// イベントコールバックのバインド
	auto EventManager = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	EventManager->Register(
		EGameEvent::PlayerRespawn, 
		this, 
		&ARoomBase::OnPlayerRespawn
	);
}

// ゲーム終了時処理
void ARoomBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// プレイヤーがリスポーンしたときに呼ばれる関数
void ARoomBase::OnPlayerRespawn(const FString& Message)
{
	// プレイヤーを
	AActor* PlayerActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	PlayerActor->SetActorLocation(PlayerRespawnPoint);
}

// ルームをリセットされた時呼び出される関数
void ARoomBase::ResetRoom()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetRoom called for %s"), *GetName());
	// 子アクターの位置を初期位置にリセット
	for (const auto& Pair : ChildInitPositions)
	{
		UChildActorComponent* ChildComponent = Pair.Key;

		if (!IsValid(ChildComponent))
		{
			continue;
		}

		AActor* ChildActor = ChildComponent->GetChildActor();
		UE_LOG(LogTemp, Warning, TEXT("ChildActor %s is Reset"), *ChildActor->GetName());
		if (!IsValid(ChildActor))
		{
			continue;
		}

		const FVector& InitialPosition = Pair.Value;

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("ResetActor Position %s"),
			*ChildActor->GetName()
		);

		ChildActor->SetActorLocation(InitialPosition);
	}

	OnPlayerRespawn("");
}

// リスポーンポイントを設定する
void ARoomBase::SetRespawnPoint(const FVector& Location)
{
	PlayerRespawnPoint = Location;
}

// タイルマップのバウンディングを取得する
FBox ARoomBase::GetTilemapBounds() const
{
	return Tilemap->Bounds.GetBox();
}