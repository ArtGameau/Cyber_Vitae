// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CVWeaponsComponent.generated.h"

class ACVWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBER_VITAE_API UCVWeaponsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCVWeaponsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SpawnWeapons();

	void SpawnNewWeapon();

	int32 CurrentWeaponPlace;

	//UPROPERTY(EditDefaultsOnly, Category = "Player")
		int32 CurrentStackSize;

	int32 MaxStackSize;

	//UPROPERTY(EditDefaultsOnly, Category = "Player")
		TArray<TSubclassOf<ACVWeapon>> EquippedWeaponClasses;

	TArray<ACVWeapon*> EquippedWeapons;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName WeaponAttachSocketName;

	bool bIsTank;

	int32 TankBonus;

public:

	ACVWeapon* NextWeapon();
	ACVWeapon* PreviousWeapon();
	ACVWeapon* FirstWeapon();
	
	//Called to fill weapon magazine
	void FindAndReload(TSubclassOf<ACVWeapon> WeaponType);

	bool AddWeapon(TSubclassOf<ACVWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable)
		TSubclassOf<ACVWeapon> Remove(int32 index);

	UFUNCTION(BlueprintCallable)
		TArray<ACVWeapon*> GetWeapons();
	
	//used when choosing character class
	void SetMaxStackSize(int32 NewMax);

	//setting up tank class
	void SetTank(int32 NewMaxStack, int32 DamageBonus);

};
