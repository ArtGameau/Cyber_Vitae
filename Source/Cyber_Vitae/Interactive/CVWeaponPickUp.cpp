// Fill out your copyright notice in the Description page of Project Settings.


#include "CVWeaponPickUp.h"
#include "CVGameMode.h"
#include "Engine/World.h"
#include "../Weapons/CVWeapon.h"
#include "../Components/CVWeaponsComponent.h"
#include "../Characters/CVCharacter.h"

ACVWeaponPickUp::ACVWeaponPickUp() {
	
	DamageInfo = "No damage";
	RangeInfo = "No range";

}

void ACVWeaponPickUp::BeginPlay()
{
	//find weapon type information in data table and initialise members of this class
	ACVGameMode* GameMode = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* WeaponTable = GameMode->GetWeaponDB();

	if (WeaponTable)
	{
		FWeaponItem* WeaponInfo = WeaponTable->FindRow<FWeaponItem>(WeaponID, "");

		if (WeaponInfo) {
			DamageInfo = FString::SanitizeFloat(WeaponInfo->Damage);
			RangeInfo = FString::FromInt(WeaponInfo->Range);
		}
	}
}

FString ACVWeaponPickUp::GetInfoText()
{
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
