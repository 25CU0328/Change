// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGoal.h"
#include "Change/Event/GameEventSubsystem.h"
// コンストラクター
ARoomGoal::ARoomGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(
        TEXT("GoalBox")
    );
}

// ゲーム開始時や生成された時の処理
void ARoomGoal::BeginPlay()
{
	Super::BeginPlay();
	
    if (!BoxComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("BoxComponent does not exist"));
        return;
    }
    BoxComponent->OnComponentBeginOverlap.AddDynamic(
        this,
        &ARoomGoal::OnBeginOverlap
    );
}

// 衝突開始時呼び出される
UFUNCTION()
void ARoomGoal::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
    UE_LOG(LogTemp, Warning, TEXT("Room Goal BeginOverlap"));
    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    if (OtherActor->ActorHasTag(FName("Player")))
    {
        auto EventSystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

        EventSystem->SendEvent(
            EGameEvent::RoomClear,
            ""
        );
    }
}

