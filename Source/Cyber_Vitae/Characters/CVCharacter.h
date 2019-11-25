// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CVCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ACVWeapon;
class UCVHealthComponent;

UCLASS()
class CYBER_VITAE_API ACVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACVCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void BeginZoom();
	void EndZoom();

	void NextWeapon();
	void PreviousWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCVHealthComponent* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* ZoomedSpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* ZoomedCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ACVWeapon* EquippedWeapon;

	void SpawnWeapon();

	int32 CurrentWeaponPlace;

	int32 WeaponStackSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TArray<TSubclassOf<ACVWeapon>> EquipedWeaponClasses;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UFUNCTION()
		void OnHealthChanged(UCVHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType,
			class AController* InstigatedBy, AActor* DamageCauser);

	//Pawn died previously
	UPROPERTY(BlueprintReadOnly, Category = "Player")
		bool bDied;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bWantsToZoom;

	UPROPERTY(VisibleAnywhere, Category = "Player")
		float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

};
