// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CVGameMode.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UDataTable* GetItemDB() const;

	UDataTable* GetWeaponDB() const;
	
protected:

	UPROPERTY(EditDefaultsOnly)
		UDataTable* ItemDB;

	UPROPERTY(EditDefaultsOnly)
		UDataTable* WeaponDB;
};
