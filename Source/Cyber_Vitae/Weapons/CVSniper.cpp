// Fill out your copyright notice in the Description page of Project Settings.


#include "CVSniper.h"

ACVSniper::ACVSniper()
{
	RateOfFire = 60;
	BulletSpread = 0.1f;
	bCanZoom = true;
	BaseDamage = 50;
	ZoomedFOV = 0.50f;
}

void ACVSniper::BeginPlay()
{
	Super::BeginPlay();

	

	TimeBetweenShots = 60 / RateOfFire;
	
}
