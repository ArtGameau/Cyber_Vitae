// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVProjectile.generated.h"

class UStaticMeshComponent;
class URadialForceComponent;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class CYBER_VITAE_API ACVProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACVProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Explode();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	//UPROPERTY(VisibleAnywhere, Category = "Components")
	//	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* RadialForceComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UProjectileMovementComponent* MovementComp;


	UPROPERTY(BlueprintReadOnly, Category="Projectile")
		float BaseDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
		float BonusDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		float ExplosionDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		UParticleSystem* ExplosionEffect;
	
	//intensity of impulse applied to barrel when it explodes
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		bool bImpactProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		bool bFleshProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
		int32 ImpactNum;

	FTimerHandle TimerHandle_Explode;

public:

	// Called every frame
	void Tick(float DeltaTime) override;

	void SetDamage(float NewDamage);
	
	void SetBonus(float NewBonus);

	void SetRadius(float NewRadius);
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
