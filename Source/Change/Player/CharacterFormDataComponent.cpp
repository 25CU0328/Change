// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFormDataComponent.h"

// Sets default values for this component's properties
UCharacterFormDataComponent::UCharacterFormDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterFormDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterFormDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 変身データを取得する
FCharacterFormData UCharacterFormDataComponent::GetData(const ETransformState TargetTransformState) const
{
	// 管理用のマップからデータを取得する
	auto* Data = DataMap.Find(TargetTransformState);
	if (Data == nullptr)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("TransformData not found for TransformState: %d"),
			static_cast<int32>(TargetTransformState)
		);
		return FCharacterFormData();
	}

	return *Data;
}

