// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/CVPickUp.h"
#include "CVDataPickUp.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVDataPickUp : public ACVPickUp
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Data Pickup")
		void CVDataTaken();

	void Interact(ACVCharacter* Character) override;
	
};
