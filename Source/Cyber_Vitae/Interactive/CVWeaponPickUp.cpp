// Fill out your copyright notice in the Description page of Project Settings.


#include "CVWeaponPickUp.h"
#include "CVGameMode.h"
#include "Engine/World.h"
#include "../Weapons/CVWeapon.h"
#include "../Components/CVWeaponsComponent.h"
#include "../Characters/CVCharacter.h"

ACVWeaponPickUp::ACVWeaponPickUp() {
}

FString ACVWeaponPickUp::GetInfoText()
{
	ACVGameMode* GameMode = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* WeaponTable = GameMode->GetWeaponDB();

	if (WeaponTable)
	{
		FWeaponItem* WeaponInfo = WeaponTable->FindRow<FWeaponItem>(WeaponID, "");

		if (WeaponInfo) {
			DamageInfo = WeaponInfo->Damage.ToString();
			RangeInfo = WeaponInfo->Range.ToString();
		}
	}

	return FString::Printf(TEXT("Damage: %s \nRange: %s"), *DamageInfo, *RangeInfo);
}

void ACVWeaponPickUp::Interact(ACVCharacter * Character)
{
	bIsInUse = true;

	UCVWeaponsComponent* WeaponsInventory = Cast<UCVWeaponsComponent>(Character->GetComponentByClass(UCVWeaponsComponent::StaticClass()));

	if ( WeaponsInventory && WeaponsInventory->AddWeapon(WeaponClass)) {
		Destroy();
	}
}
