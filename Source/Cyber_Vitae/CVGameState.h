// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CVGameState.generated.h"

/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		bool bIsTankAreaFinished;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		bool bIsJetpackAreaFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		bool bIsHackerAreaFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		bool bIsTankAreaActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		bool bIsJetpackAreaActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		bool bIsHackerAreaActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameState")
		uint8 CVPartsTaken;
	
};
