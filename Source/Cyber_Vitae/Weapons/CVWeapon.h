// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class CYBER_VITAE_API ACVWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVWeapon();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USkeletalMeshComponent* MeshComp;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
