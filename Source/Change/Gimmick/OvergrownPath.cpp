// Fill out your copyright notice in the Description page of Project Settings.


#include "OvergrownPath.h"
#include "Change/MainCharacter.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AOvergrownPath::AOvergrownPath()
	: bPlayerAbility(false)
	, Timer(0.f)
	, m_pPlayerChara(NULL)
	
{

	// ルートに置くベースオブジェクト生成
	pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = pBase;

	// コリジョン判定用ボックスコンポ―ネット生成
	pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	pBoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AOvergrownPath::BeginPlay()
{
	Super::BeginPlay();
	// スプライトコンポネントを取得する
	SpriteComponent = GetComponentByClass<UPaperSpriteComponent>();
	if (pBoxComp)
	{
		// 衝突開始イベントのバインディング
		pBoxComp->OnComponentBeginOverlap.AddDynamic(
			this,
			&AOvergrownPath::OnOverlapBegin
		);

		// 衝突終了イベントのバインディング
		pBoxComp->OnComponentEndOverlap.AddDynamic(
			this,
			&AOvergrownPath::OnOverlapEnd
		);
	}
}

// 毎フレームの更新処理
void AOvergrownPath::Tick(float DeltaTime)
{
	if (IsUesAbility)
	{
		Gimmicke(DeltaTime);
	}
}

// 衝突開始時呼び出される
void AOvergrownPath::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if(OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			m_pPlayerChara = Cast<AMainCharacter>(OtherActor);
			UE_LOG(LogTemp, Warning, TEXT("Player"));
			if (m_pPlayerChara)
			{
				bPlayerAbility = m_pPlayerChara->GetbChargeing();
				UE_LOG(LogTemp, Warning, TEXT("Player"));

				if (bPlayerAbility)
				{
					m_pPlayerChara->SetintoOvergrownPath(IsUesAbility);
					SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				}

			}
		}
	}
}

// 衝突終了時呼び出される
void AOvergrownPath::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
) 
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (!m_pPlayerChara) {
				m_pPlayerChara = Cast<AMainCharacter>(OtherActor);
			}

			if (!m_pPlayerChara)
			{
				return;
			}
			IsUesAbility = true;
		}
	}
}


// ギミックの能力
void AOvergrownPath::Gimmicke(float DeltaTime)
{
	Timer += DeltaTime;
	if (Timer >= 1.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player"));
		SpriteComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Timer = 0.f;
		IsUesAbility = false;
	}
	
}
