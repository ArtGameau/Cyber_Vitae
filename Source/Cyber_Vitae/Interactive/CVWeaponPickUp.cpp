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
			WeaponClass = WeaponInfo->WeaponClass;
			DamageInfo = FString::SanitizeFloat(WeaponInfo->Damage);
			RangeInfo = FString::FromInt(WeaponInfo->Range);
			BonusInfo = FString::SanitizeFloat(WeaponInfo->MaxDamageBonus);
		}
	}
}

FString ACVWeaponPickUp::GetInfoText()
{
	return FString::Printf(TEXT("Damage: %s \nRange: %s\nMax Bonus: %s"), *DamageInfo, *RangeInfo, *BonusInfo);
}

void ACVWeaponPickUp::Interact(ACVCharacter * Character)
{
	bIsInUse = true;

	UE_LOG(LogTemp, Log, TEXT("Picking up weapon!"));

	UCVWeaponsComponent* WeaponsInventory = Cast<UCVWeaponsComponent>(Character->GetComponentByClass(UCVWeaponsComponent::StaticClass()));

	//if weapon is successfully added to inventory destroy pick up actor if not indicate that it is not in use anymore so it can be picked up again
	if ( WeaponsInventory && WeaponsInventory->AddWeapon(WeaponClass)) {
		Destroy();
	}
	else {
		bIsInUse = false;
	}
}
