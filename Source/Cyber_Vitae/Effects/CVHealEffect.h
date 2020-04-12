// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effects/CVBaseEffect.h"
#include "CVHealEffect.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVHealEffect : public ACVBaseEffect
{
	GENERATED_BODY()

public:
	ACVHealEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Intensity;


public:
		virtual void Use() override;

		virtual void EndEffect() override;
};
