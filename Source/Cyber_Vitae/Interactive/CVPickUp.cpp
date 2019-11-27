// Fill out your copyright notice in the Description page of Project Settings.


#include "CVPickUp.h"

ACVPickUp::ACVPickUp()
{
	ItemID = FName("Please enter an ID");
}

void ACVPickUp::Interact(ACVCharacter* Character)
{
	Destroy();
}

