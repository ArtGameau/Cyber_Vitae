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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ACVWeapon> WeaponClass;

	FString DamageInfo;

	FString RangeInfo;


	UFUNCTION(BlueprintCallable, Category = "Interactive")
		FString GetInfoText();


public:
	/*
	 * called when player character interacts with this
	 * can be implemented in blueprints if needed
	 */
	void Interact(ACVCharacter* Character) override;
};
