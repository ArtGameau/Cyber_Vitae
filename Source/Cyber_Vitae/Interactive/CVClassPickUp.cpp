// Fill out your copyright notice in the Description page of Project Settings.


#include "CVClassPickUp.h"
#include "../Characters/CVCharacter.h"

ACVClassPickUp::ACVClassPickUp() {
}

void ACVClassPickUp::Interact(ACVCharacter * Character)
{
	if (Character->SetupCharacterClass(CharacterClass)) {
		Destroy();
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Character Class is already set!"));
}
