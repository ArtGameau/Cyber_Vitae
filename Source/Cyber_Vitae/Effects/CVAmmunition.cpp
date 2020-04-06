// Fill out your copyright notice in the Description page of Project Settings.


#include "CVAmmunition.h"
#include "Characters/CVCharacter.h"
#include "Weapons/CVWeapon.h"
ACVAmmunition::ACVAmmunition() {

}

void ACVAmmunition::Use()
{
	ACVCharacter* MyOwner = Cast<ACVCharacter>(GetOwner());

	if (MyOwner) {

		MyOwner->FindAndReload(WeaponType);
	}
}
