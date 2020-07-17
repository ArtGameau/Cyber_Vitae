// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive/CVInteractiveActor.h"
#include "CVSlidingDoor.generated.h"

UENUM(BlueprintType)
enum class EDoorStateEnum :uint8 {

	DS_Open				UMETA(DisplayName = "Open"),
	DS_Closed			UMETA(DisplayName = "Closed"),
	DS_Moving			UMETA(DisplayName = "MOving")
};
/**
 * 
 */
UCLASS()
class CYBER_VITAE_API ACVSlidingDoor : public ACVInteractiveActor
{
	GENERATED_BODY()

public:
	ACVSlidingDoor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Door")
		EDoorStateEnum DoorState;

	UPROPERTY(EditDefaultsOnly, Category = "Door")
		float DoorWidth;

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interactive")
		void Interact(ACVCharacter* Character) override;
};
