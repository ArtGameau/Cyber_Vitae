// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/CVInteractiveActor.h"
#include "CVPickUp.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVPickUp : public ACVInteractiveActor
{
	GENERATED_BODY()

public:
	ACVPickUp();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

public:
	/*
	 * called when player character interacts with this
	 * can be implemented in blueprints if needed
	 */
		void Interact(ACVCharacter* Character) override;
	
};
