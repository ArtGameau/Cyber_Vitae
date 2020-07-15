// Fill out your copyright notice in the Description page of Project Settings.


#include "CVWeaponPickUp.h"
#include "../Components/CVWeaponsComponent.h"
#include "../Characters/CVCharacter.h"

ACVWeaponPickUp::ACVWeaponPickUp() {

}

void ACVWeaponPickUp::Interact(ACVCharacter * Character)
{
	bIsInUse = true;

	UCVWeaponsComponent* WeaponsInventory = Cast<UCVWeaponsComponent>(Character->GetComponentByClass(UCVWeaponsComponent::StaticClass()));

	if ( WeaponsInventory && WeaponsInventory->AddWeapon(WeaponClass)) {
		Destroy();
	}
}
