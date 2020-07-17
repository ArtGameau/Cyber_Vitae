// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/CVPickUp.h"
#include "CVClassPickUp.generated.h"

enum class ECharClassEnum :uint8;

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVClassPickUp : public ACVInteractiveActor
{
	GENERATED_BODY()

public:
	ACVClassPickUp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharClassEnum CharacterClass;

public:
	/*
	 * called when player character interacts with this
	 * can be implemented in blueprints if needed
	 */
	void Interact(ACVCharacter* Character) override;
};
	

