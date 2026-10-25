// Fill out your copyright notice in the Description page of Project Settings.


#include "Stone.h"
#include "Components/BoxComponent.h"
#include "Change/Event/GameEventSubsystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AStone::AStone()
	:m_pPlayerChara(NULL)
	,m_IsUesAbility(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ルートに置くベースオブジェクト生成
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	m_pBase->SetMobility(EComponentMobility::Movable);
	RootComponent = m_pBase;

	// コリジョン判定用ボックスコンポ―ネット生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);
	// スプライトコンポネントを取得する
	
	

	

	//UGameEventSubsystem* pEventSubsystem = Cast<UGameEventSubsystem>(GetGameInstance());
	//pEventSubsystem->Register(EGameEvent::UseAbility, this, &AStone::Coulbauk);
	NeedAnimalType = ETransformState::Boar;
	

}

// Called when the game starts or when spawned
void AStone::BeginPlay()
{
	Super::BeginPlay();


	if (m_pBoxComp)
	{
		// 接触時のイベント登録
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AStone::OnOverlapBegin);
		// 離れ始めた時のイベント登録
		m_pBoxComp->OnComponentEndOverlap.AddDynamic(this, &AStone::OnOverlapEnd);
	}

	SpriteComponent = FindComponentByClass<UPaperSpriteComponent>();
	if (SpriteComponent)
	{
		// 参照成功！ここでBP側のスプライトを操作できます
		UE_LOG(LogTemp, Warning, TEXT("Sprite Component を発見しました"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Sprite Component が見つかりません！"));
	}
}

// Called every frame
void AStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsUesAbility) {
		SetMovement(DeltaTime);
	}
	
	
}

// 接触し始めた時に呼ばれるイベント関数
void AStone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			m_pPlayerChara = Cast<AMainCharacter>(OtherActor);
			if (NeedAnimalType == m_pPlayerChara->GetTransformState())
			{
				m_pPlayerChara->SetCanuesAbility(true);
				SetUseAbility(true);
				//デバッグ確認用
				UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));
				m_pPlayerChara->SetMove(true);
			}

		}
	}
}

// 離れ始めた時に呼ばれるイベント関数
void AStone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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
			m_pPlayerChara->SetCanuesAbility(false);
			SetUseAbility(false);
			//デバッグ確認用
			UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd"));
		}
	}
}

//移動値を加算
void AStone::SetMovement(float _delteTime)
{
	if (!m_pPlayerChara)
	{
		return;
	}

	if (m_pPlayerChara->GetIsUesAbility())
	{
		BeginMovement(m_pPlayerChara);
		colader();
	}
	else
	{
		EndMovement(m_pPlayerChara);
	}

	

	/*USkeletalMeshComponent* pPlayermesh = Cast<AMainCharacter>(m_pPlayerChara)->GetMesh();

	FVector PlayerMove = pPlayermesh->GetComponentLocation();
	//プレイヤーの前に、オブジェクトのサイズを考慮した位置に配置したい
	FVector newLocation = PlayerMove + pPlayermesh->GetForwardVector() * (100 + 100) / 2;

	//オブジェクトの座標・回転を設定
	SetActorLocation(FVector(newLocation));*/

	

}

// アビリティ使用された時の関数
void AStone::BeginMovement(AMainCharacter* _maincharactor)
{
	if (!_maincharactor)return;

	//アタッチルールを設定
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::KeepWorld,		//位置
		EAttachmentRule::KeepWorld,		//回転
		EAttachmentRule::KeepWorld,		//スケール
		false							//物理シミュレーション
	);

	//プレイヤーにアタッチする
	this->AttachToActor(_maincharactor, AttachmentRules);
	SpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// アビリティ終了された時の関数
void AStone::EndMovement(AMainCharacter* _maincharactor)
{
	if (!_maincharactor)return;

	//アタッチ解除時のルール
	FDetachmentTransformRules DetachRules(
		EDetachmentRule::KeepWorld,
		false
	);

	this->DetachFromActor(DetachRules);
	SpriteComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_pPlayerChara->SetMove(false);
}


void AStone::Coulbauk(const FString & Message)
{
	// 画面に「ポーションを拾いました！」などのテキストを表示する処理
	UE_LOG(LogTemp, Log, TEXT("UI表示: %s"), *Message);

	BeginMovement(m_pPlayerChara);
}

void AStone::colader()
{
    //レイを飛ばす
    //レイの始点はActorの位置
    FVector Start = GetActorLocation();
    //レイの終点はActorから前方向の一定距離
    FVector End = GetActorLocation() + 100 * 1.5;

    //デバッグ確認用のラインを描画
    //DrawDebugLine(GetWorld(),Start,End,FColor::Green,false,1.0f);

    //コリジョン判定で無視する項目を指定（今回はこのActor自分自身。thisポインタで指定）
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);

    //ヒットした（＝コリジョン判定を受けた）オブジェクトを格納する変数
    FHitResult OutHit;

    //レイを飛ばし、全てのオブジェクトに対してコリジョン判定を行う
    //isHitは、ヒットした場合にtrueになる
    bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End,
	   ECC_WorldStatic, CollisionParams);

    //ヒットするオブジェクトがある場合
    if (isHit)
    {
	   UE_LOG(LogTemp, Warning, TEXT("OnWool"));
	   m_pPlayerChara->SetMove(true);
    }
	else
	{
		m_pPlayerChara->SetMove(false);
	}
}