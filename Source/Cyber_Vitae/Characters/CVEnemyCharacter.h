// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CVEnemyCharacter.generated.h"

class ACVWeapon;
class UCVHealthComponent;


UCLASS()
class CYBER_VITAE_API ACVEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACVEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void Reload();

	void StartFire();
	void StopFire();

	void SpawnWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCVHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<ACVWeapon> EquippedWeaponClass;

	ACVWeapon* EquippedWeapon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName WeaponAttachSocketName;

	UFUNCTION()
		void OnHealthChanged(UCVHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	//Pawn died previously
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bDied;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
