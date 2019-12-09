// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVInteractiveActor.generated.h"

class ACVCharacter;
class UStaticMeshComponent;

UCLASS()
class CYBER_VITAE_API ACVInteractiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVInteractiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	FString Action;

	UFUNCTION(BlueprintCallable, Category="Interactive")
	FString GetUseText() const;

public:
	
	/*
	 * called when player character interacts with this
	 * can be implemented in blueprints if needed
	 */
	virtual	void Interact(ACVCharacter* Character);

	virtual UStaticMeshComponent* GetMesh();
};
