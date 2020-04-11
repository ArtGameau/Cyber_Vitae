// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/CVInteractiveActor.h"
#include "CVWeaponPickUp.generated.h"

class ACVWeapon;
/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVWeaponPickUp : public ACVInteractiveActor
{
	GENERATED_BODY()
	
public:
	ACVWeaponPickUp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACVWeapon> WeaponClass;


public:
	/*
	 * called when player character interacts with this
	 * can be implemented in blueprints if needed
	 */
	void Interact(ACVCharacter* Character) override;
};
