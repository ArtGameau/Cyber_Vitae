// Fill out your copyright notice in the Description page of Project Settings.


#include "CVProjectileWeapon.h"
#include "Weapons/CVProjectile.h"
#include "Characters/CVCharacter.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ACVProjectileWeapon::ACVProjectileWeapon()
{
	RateOfFire = 60;
}

// Called when the game starts or when spawned
void ACVProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	TimeBetweenShots = 60 / RateOfFire;

}

void ACVProjectileWeapon::Fire()
{
	AActor* MyOwner = GetOwner();

	if (MyOwner && ProjectileClass && CurrentAmmo) {
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		ACVCharacter* CVOwner = Cast<ACVCharacter>(MyOwner);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		//spawn granade and set its damage and radius
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Projectile=Cast<ACVProjectile>(GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams));
		Projectile->SetOwner(this);
		Projectile->SetDamage(BaseDamage);
		Projectile->SetRadius(Range);

		//set bonus on granade only if player is tank 
		if (CVOwner && CVOwner->GetCharClass() == ECharClassEnum::CE_Tank) {
			Projectile->SetBonus(BonusDamage);
		}

		if (MuzzleEffect) {
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}

		CurrentAmmo--;
	}
}

