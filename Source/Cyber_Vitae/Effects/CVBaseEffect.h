// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVBaseEffect.generated.h"

class ACVCharacter;

UCLASS()
class CYBER_VITAE_API ACVBaseEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVBaseEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//called when effect begins it's use
	FTimerHandle TimerHandle_EffectDuration;

public:

	UFUNCTION(BlueprintCallable)
	virtual void Use();

	virtual void EndEffect();

	UPROPERTY(EditDefaultsOnly, Category="Effect")
	float Duration;
};
