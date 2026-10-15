// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterFormData.h"

#include "CharacterFormDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHANGE_API UCharacterFormDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterFormDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 変身データを取得する
	FCharacterFormData GetData(const ETransformState TargetTransformState) const;

private:
	// 変身データを管理する配列
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta= (AllowPrivateAccess = "true"))
	TMap<ETransformState, FCharacterFormData> DataMap;
};
