// Fill out your copyright notice in the Description page of Project Settings.


#include "CVDataPickUp.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/CVInventoryComponent.h"


//set green outline when data pick up enters play so it would be more visible
void ACVDataPickUp::BeginPlay() {

	Super::BeginPlay();
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(252);
}

void ACVDataPickUp::Interact(ACVCharacter* Character)
{
	bIsInUse = true;

	UCVInventoryComponent* Inventory = Cast<UCVInventoryComponent>(Character->GetComponentByClass(UCVInventoryComponent::StaticClass()));

	Inventory->Add(ItemID);

	CVDataTaken();

	Destroy();
}
