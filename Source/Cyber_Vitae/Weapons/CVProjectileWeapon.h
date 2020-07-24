// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CVWeapon.h"
#include "CVProjectileWeapon.generated.h"

class ACVProjectile;

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVProjectileWeapon : public ACVWeapon
{
	GENERATED_BODY()

public:
	ACVProjectileWeapon();
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileWeapon")
		TSubclassOf<ACVProjectile> ProjectileClass;

	ACVProjectile* Projectile;
	
};
