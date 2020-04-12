// Fill out your copyright notice in the Description page of Project Settings.


#include "CVInteractiveActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACVInteractiveActor::ACVInteractiveActor()
{
	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	
	Name = "Name not set";
	Action = "Interact";

}

// Called when the game starts or when spawned
void ACVInteractiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}


FString ACVInteractiveActor::GetUseText() const
{
	return FString::Printf(TEXT("%s : Press 'E' to %s"), *Name, *Action);
}

void ACVInteractiveActor::Interact(ACVCharacter* Character)
{
}

UStaticMeshComponent* ACVInteractiveActor::GetMesh()
{
	return MeshComp;
}



