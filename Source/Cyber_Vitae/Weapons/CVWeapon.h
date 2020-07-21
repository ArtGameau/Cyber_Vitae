// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CVWeapon.generated.h"

class USkeletalMeshComponent;
class UCameraShake;
class UDamageType;

//WEAPON INFO ITEM

USTRUCT(BlueprintType)
struct FWeaponItem : public FTableRowBase
{
	GENERATED_BODY()

public:

	FWeaponItem()
	{
		Name = FText::FromString("No name");
		Damage = FText::FromString("No damage");
		MaxDamageBonus = FText::FromString("No bonus");
		Ammo= FText::FromString("No ammo");
		BulletSpread = FText::FromString("No bullet spread");
		RateOfFire = FText::FromString("No rate of fire");
		Range = FText::FromString("No range");
		Description = FText::FromString("Please enter description for this item.");

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ACVWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ACVWeaponPickUp> PickUpClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText MaxDamageBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsHackable;

	bool operator==(const FWeaponItem Item) const
	{
		if (WeaponID == Item.WeaponID)
		{
			return true;
		}

		return false;
	}
};


UCLASS()
class CYBER_VITAE_API ACVWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVWeapon();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName WeaponID;

	virtual void Fire();
	
	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	//leftover ammunition
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		int32 CurrentAmmo;

	//RPM - Bullets per minute fired by weapon
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float RateOfFire;

	//Bullet spread in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
		float BulletSpread;

	//Derived from RateOfFire
	float TimeBetweenShots;

	void PlayFireEffect(FVector EndPoint);

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		int32 MagazineSize;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BonusDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		int32 Range;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShake> FireCamShake;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		bool bCanZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName AmmoID;

	void SetBonusDamage(float NewBonus);

	void StartFire();
	void StopFire();

	void ActivateWeapon();
	void DeactivateWeapon();

	void Reload();
};
