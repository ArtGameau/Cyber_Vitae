// Fill out your copyright notice in the Description page of Project Settings.


#include "CVWeaponsComponent.h"
#include "../Weapons/CVWeapon.h"
#include "../Characters/CVCharacter.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UCVWeaponsComponent::UCVWeaponsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeaponAttachSocketName = "WeaponSocket";

	CurrentWeaponPlace = 0;
	CurrentStackSize = 1;
	MaxStackSize = 4;

	EquippedWeaponClasses.SetNum(MaxStackSize);
	EquippedWeapons.SetNum(MaxStackSize);
}


// Called when the game starts
void UCVWeaponsComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
	
}

void UCVWeaponsComponent::SpawnWeapons()
{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* MyOwner = GetOwner();
		ACVCharacter* ChOwner = Cast<ACVCharacter>(MyOwner);

		for (int i = 0; i < CurrentStackSize; i++) {

			EquippedWeapons[i] = GetWorld()->SpawnActor<ACVWeapon>(EquippedWeaponClasses[i], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			if (EquippedWeapons[i]) {

				UE_LOG(LogTemp, Log, TEXT("Weapon spawned!"));

				if (MyOwner && ChOwner){
					EquippedWeapons[i]->SetOwner(MyOwner);
					EquippedWeapons[i]->AttachToComponent(Cast<USceneComponent>(ChOwner->GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
				}
			}
		}
}

void UCVWeaponsComponent::SpawnNewWeapon()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* MyOwner = GetOwner();
	ACVCharacter* ChOwner = Cast<ACVCharacter>(MyOwner);

	EquippedWeapons[CurrentStackSize] = GetWorld()->SpawnActor<ACVWeapon>(EquippedWeaponClasses[CurrentStackSize], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (EquippedWeapons[CurrentStackSize]) {

		UE_LOG(LogTemp, Log, TEXT("Weapon spawned!"));

		if (MyOwner && ChOwner) {
			EquippedWeapons[CurrentStackSize]->SetOwner(MyOwner);
			EquippedWeapons[CurrentStackSize]->AttachToComponent(Cast<USceneComponent>(ChOwner->GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}
}

ACVWeapon* UCVWeaponsComponent::NextWeapon()
{
	//deactivate current weapon
	EquippedWeapons[CurrentWeaponPlace]->DeactivateWeapon();
	//moving to next weapon and activating
	CurrentWeaponPlace = (CurrentWeaponPlace + 1) % CurrentStackSize;
	EquippedWeapons[CurrentWeaponPlace]->ActivateWeapon();

	//return next weapon
	return EquippedWeapons[CurrentWeaponPlace];
}

ACVWeapon* UCVWeaponsComponent::PreviousWeapon()
{
	//deactivate current weapon
	EquippedWeapons[CurrentWeaponPlace]->DeactivateWeapon();
	//moving to next weapon and activating
	CurrentWeaponPlace = (CurrentWeaponPlace - 1 + CurrentStackSize) % CurrentStackSize;
	EquippedWeapons[CurrentWeaponPlace]->ActivateWeapon();

	//return next weapon
	return EquippedWeapons[CurrentWeaponPlace];
}

ACVWeapon* UCVWeaponsComponent::FirstWeapon()
{	
	return EquippedWeapons[CurrentWeaponPlace];
}

void UCVWeaponsComponent::FindAndReload(TSubclassOf<ACVWeapon> WeaponType)
{
	int index;
	if (EquippedWeaponClasses.Find(WeaponType, index)) {

		EquippedWeapons[index]->Reload();
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Wrong Weapon Type!"));
	}
}

bool UCVWeaponsComponent::AddWeapon(TSubclassOf<ACVWeapon> WeaponClass)
{
	if (CurrentStackSize < MaxStackSize) {
		EquippedWeaponClasses[CurrentStackSize] = WeaponClass;
		SpawnNewWeapon();
		CurrentStackSize++;

		return true;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("You are carrying too many weapons!"));
		
		return false;
	}
}

void UCVWeaponsComponent::Remove(int32 index)
{
	int32 i;

	//moving current weapon that the playing is holding is not permited
	if (index != CurrentWeaponPlace) {

		//destroying weapon
		EquippedWeapons[index]->Destroy();

		//moving all next weapons backwards so array can stay continually filled
		for (i = index + 1; i < CurrentStackSize; i++) {
			EquippedWeaponClasses[i - 1] = EquippedWeaponClasses[i];
			EquippedWeapons[i - 1] = EquippedWeapons[i];
		}

		//last place in array is now supposed to be empty
		EquippedWeapons[CurrentStackSize - 1] = NULL;
		EquippedWeaponClasses[CurrentStackSize - 1] = NULL;
		CurrentStackSize--;

		//if we removed weapon that was before current weapon in array 
		//then current weapon index also moved back one place
		if (index < CurrentWeaponPlace) {
			CurrentWeaponPlace--;
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("You can't remove weapon you are holding!"));
	}
	
}

TArray<ACVWeapon*> UCVWeaponsComponent::GetWeapons()
{
	return EquippedWeapons;
}



