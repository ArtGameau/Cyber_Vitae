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
		Damage = 0;
		MaxDamageBonus = 0;
		Ammo = 0;
		BulletSpread = 0;
		RateOfFire = 0;
		Range = 0;
		bCanZoom = false;
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
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxDamageBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName AmmoID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Range;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanZoom;

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
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		float RateOfFire;

	//Bullet spread in degrees
	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
		float BulletSpread;

	//Derived from RateOfFire
	float TimeBetweenShots;

	void PlayFireEffect(FVector EndPoint);

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		int32 MagazineSize;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		float BaseDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		float BonusDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
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
	UPROPERTY(BlueprintReadOnly)
		FText Name;

	UPROPERTY(BlueprintReadOnly)
		UTexture2D* Thumbnail;

	UPROPERTY(BlueprintReadOnly)
		FText Description;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
		bool bCanZoom;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
		FName AmmoID;

	void SetBonusDamage(float NewBonus);

	void StartFire();
	void StopFire();

	void ActivateWeapon();
	void DeactivateWeapon();

	void Reload();
};
