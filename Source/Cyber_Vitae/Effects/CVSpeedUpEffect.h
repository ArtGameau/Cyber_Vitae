// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effects/CVBaseEffect.h"
#include "CVSpeedUpEffect.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVSpeedUpEffect : public ACVBaseEffect
{
	GENERATED_BODY()
	
public:
	ACVSpeedUpEffect();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Boost;


public:
	virtual void Use() override;

	virtual void EndEffect() override;


};
