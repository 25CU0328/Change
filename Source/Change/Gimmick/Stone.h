// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Change/ETransformState.h"	
#include "GameFramework/Actor.h"
#include "Change/MainCharacter.h"
#include "Stone.generated.h"

//前方宣言
class UBoxComponent;
class UMeshComponent;
class AMainCharacter;
class UPaperSpriteComponent;

UCLASS()
class CHANGE_API AStone : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// 接触し始めた時に呼ばれるイベント関数
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 離れ始めた時に呼ばれるイベント関数
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetUseAbility(bool _uesability){ m_IsUesAbility = _uesability; }

	void Coulbauk(const FString & Message);
	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> SpriteComponent;

private:

	//移動値を加算
	void SetMovement(float _delteTime);

	// アビリティ使用された時の関数
	void BeginMovement(AMainCharacter* _maincharactor);
	// アビリティ終了された時の関数
	void EndMovement(AMainCharacter* _maincharactor);

public:
	// 接触用のコリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* m_pBoxComp;

	// ベースコンポ―ネット
	UPROPERTY()
	USceneComponent* m_pBase;

	void colader();

	

private:
	UPROPERTY()
	AMainCharacter* m_pPlayerChara;
	ETransformState NeedAnimalType;
	
	

	//アビリティ使用状態フラグ
	UPROPERTY()
	bool m_IsUesAbility;
};
