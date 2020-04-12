// Fill out your copyright notice in the Description page of Project Settings.


#include "CVBaseEffect.h"
#include "Characters/CVCharacter.h"

// Sets default values
ACVBaseEffect::ACVBaseEffect()
{
	Duration = 0.0f;
}

// Called when the game starts or when spawned
void ACVBaseEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ACVBaseEffect::Use()
{
}

void ACVBaseEffect::EndEffect()
{
}




