// Fill out your copyright notice in the Description page of Project Settings.

#include "ARoomBase.h"

ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
