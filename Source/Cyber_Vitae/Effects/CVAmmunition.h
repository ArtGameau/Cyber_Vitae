// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effects/CVBaseEffect.h"
#include "CVAmmunition.generated.h"


class ACVWeapon;
/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVAmmunition : public ACVBaseEffect
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACVAmmunition();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammunition")
	TSubclassOf<ACVWeapon> WeaponType;

public:

		virtual void Use() override;
};
