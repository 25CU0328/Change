// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h" // 追加
#include "InputAction.h" // 追加
#include "InputActionValue.h" // 追加
#include "MainCharacter.generated.h"



// 入力関連クラスの前方宣言
class UIInputMappingContext;
class UInputAction;

UCLASS()
class CHANGE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _playerInputComponent) override;

	// 入力関連
private:
	// IAのマッピング
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* m_mappingContext;

	// 移動のIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* m_IA_playerMove;

	// プレイヤーの移動処理
	void UpdateMovement(const FInputActionValue& _value);


	// 変形のIA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* m_IA_Transform;
	// 変形ボタンが押されたばかり
	void OnTransformStarted(const FInputActionValue& _value);
	// 一定時間押されたら
	void OnTransformCompleted(const FInputActionValue& _value); 
	// 途中で離されたら
	void OnTransformCanceled(const FInputActionValue& _value); 
};
