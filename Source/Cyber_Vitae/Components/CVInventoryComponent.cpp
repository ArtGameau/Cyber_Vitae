// Fill out your copyright notice in the Description page of Project Settings.


#include "CVInventoryComponent.h"
#include "CVGameMode.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UCVInventoryComponent::UCVInventoryComponent()
{

}


// Called when the game starts
void UCVInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCVInventoryComponent::Add(FName ID)
{
	ACVGameMode* GameMode = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = GameMode->GetItemDB();

	if(ItemTable)
	{
		FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

		if(ItemToAdd)
		{
			if(!Inventory.Contains(*ItemToAdd))
			{
				Inventory.Add(*ItemToAdd);
			}
			if(InvCount.Contains(ID))
			{
				InvCount[ID]++;
			}
			else
			{
				InvCount.Add(ID, 1);
			}

			//UE_LOG(LogTemp, Log, TEXT("Added new idem to inventory. %s") ,*FString::FromInt(InvCount[ID]) );
		}
	}
}

void UCVInventoryComponent::Remove(FName ID)
{
	ACVGameMode* GameMode = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = GameMode->GetItemDB();

	if (ItemTable)
	{
		FInventoryItem* ItemToRemove = ItemTable->FindRow<FInventoryItem>(ID, "");

		if (ItemToRemove)
		{
			
			if (InvCount.Contains(ID))
			{
				InvCount[ID]--;
				if(InvCount[ID]==0)
				{
					Inventory.Remove(*ItemToRemove);
					InvCount.Remove(ID);
				}
			}
		}
	}
}

TArray<FInventoryItem> UCVInventoryComponent::GetInventory()
{
	return Inventory;
}

int32 UCVInventoryComponent::GetItemCount(FName ID)
{
	if (InvCount.Contains(ID)) {
		return InvCount[ID];
	}
	else
		return 0;
}






