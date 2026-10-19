// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomStart.h"

// Sets default values
ARoomStart::ARoomStart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomStart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

