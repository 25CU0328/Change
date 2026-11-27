// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickBase.h"
#include "PaperSpriteComponent.h"

// Sets default values
AGimmickBase::AGimmickBase()
	:IsUesAbility(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGimmickBase::BeginPlay()
{
	Super::BeginPlay();
	
	// スプライトコンポネントを取得する
	SpriteComponent = GetComponentByClass<UPaperSpriteComponent>();

	if (SpriteComponent)
	{
		// 衝突開始イベントのバインディング
		SpriteComponent->OnComponentBeginOverlap.AddDynamic(
			this,
			&AGimmickBase::OnOverlapBegin
		);

		// 衝突終了イベントのバインディング
		SpriteComponent->OnComponentEndOverlap.AddDynamic(
			this,
			&AGimmickBase::OnOverlapEnd
		);
	}
}

// Called every frame
void AGimmickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 衝突開始時呼び出される
void AGimmickBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
}

// 衝突終了時呼び出される
void AGimmickBase::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) {
}

// 対象の動物かどうかチェック
bool AGimmickBase::IsAnimalcheck(ETransformState etrasnformstate)
{
	return false;
}

// 能力使用しているか
void AGimmickBase::IsUseAbility(bool useing)
{
	IsUesAbility = useing;
}

// ギミックの能力
void AGimmickBase::Gimmick()
{

}