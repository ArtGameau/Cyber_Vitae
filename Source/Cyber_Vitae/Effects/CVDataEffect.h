// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effects/CVBaseEffect.h"
#include "CVDataEffect.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVDataEffect : public ACVBaseEffect
{
	GENERATED_BODY()

public:

	ACVDataEffect() {};

		void Use() override;

		UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ShowHUD();
	
};
