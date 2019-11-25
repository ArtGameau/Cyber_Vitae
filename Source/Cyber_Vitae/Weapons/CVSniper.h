// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CVWeapon.h"
#include "CVSniper.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVSniper : public ACVWeapon
{
	GENERATED_BODY()

public:
public:
	// Sets default values for this actor's properties
	ACVSniper();
	
protected:
	
	// Called when the game starts or when spawned
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;
	
};
