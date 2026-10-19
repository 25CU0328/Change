// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "PaperSPrite.h"
#include "PaperSpriteComponent.h"
#include "Change/Event/GameEventSubsystem.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

    // スプライトコンポネントを取得する
    SpriteComponent = GetComponentByClass<UPaperSpriteComponent>();

    // 衝突開始イベントのバインディング
    SpriteComponent->OnComponentBeginOverlap.AddDynamic(
        this,
        &AFloorSwitch::OnOverlapBegin
    );

    // 衝突終了イベントのバインディング
    SpriteComponent->OnComponentEndOverlap.AddDynamic(
        this,
        &AFloorSwitch::OnOverlapEnd
    );

    // 表示を変更
    if (NormalSprite) {
        SpriteComponent->SetSprite(NormalSprite);
        SpriteComponent->SetSpriteColor(NormalColor);
    }
}

// 衝突開始時呼び出される
UFUNCTION()
void AFloorSwitch::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComponent,
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

    if (OtherActor->ActorHasTag(FName("Stone")))
    {
        auto EventSystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

        EventSystem->SendEvent(
            PressedEvent,
            PressedEventMessage
        );

        // 表示を変更
        if (PressedSprite) {
            SpriteComponent->SetSprite(PressedSprite);
            SpriteComponent->SetSpriteColor(PressedColor);
        }
    }
}

// 衝突終了時呼び出される
UFUNCTION()
void AFloorSwitch::OnOverlapEnd(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex
)
{
    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    if (OtherActor->ActorHasTag(FName("Stone")))
    {
        auto EventSystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>();

        EventSystem->SendEvent(
            ReleaseEvent,
            ReleaseEventMessage
        );

        // 表示を変更
        if (NormalSprite) {
            SpriteComponent->SetSprite(NormalSprite);
            SpriteComponent->SetSpriteColor(NormalColor);
        }

    }
}

