// Fill out your copyright notice in the Description page of Project Settings.


#include "CVGameMode.h"
#include "CVGameState.h"


ACVGameMode::ACVGameMode()
{
	GameStateClass = ACVGameState::StaticClass();
}

UDataTable* ACVGameMode::GetItemDB() const
{
	return ItemDB;
}

UDataTable * ACVGameMode::GetWeaponDB() const
{
	return WeaponDB;
}
