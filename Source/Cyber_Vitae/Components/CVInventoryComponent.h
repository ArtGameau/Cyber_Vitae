// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CVCharacter.h"
#include "Components/ActorComponent.h"
#include "CVInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CYBER_VITAE_API UCVInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCVInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TMap<FName, int32> InvCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FInventoryItem> Inventory;

public:

	UFUNCTION(BlueprintCallable)
	void Add(FName ID);
	
	UFUNCTION(BlueprintCallable)
	bool Remove(FName  ID);

	UFUNCTION(BlueprintCallable)
		TArray<FInventoryItem> GetInventory();

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(FName  ID);



		
};
