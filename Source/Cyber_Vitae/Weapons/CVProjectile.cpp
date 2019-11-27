// Fill out your copyright notice in the Description page of Project Settings.


#include "CVProjectile.h"
#include "Cyber_Vitae.h"
#include "TimerManager.h"
#include "GameFramework/DamageType.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACVProjectile::ACVProjectile()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComp->bShouldBounce = true;
	MovementComp->bSimulationEnabled = true;
	MovementComp->bSweepCollision = true;
	MovementComp->InitialSpeed = 1500.0f;
	MovementComp->MaxSpeed = 1500.0f;
	
	BaseDamage = 50.0f;
	ExplosionDelay = 1.0f;
	bImpactProjectile = false;
	ImpactNum = 0;

}

// Called when the game starts or when spawned
void ACVProjectile::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetNotifyRigidBodyCollision(true);
	MeshComp->OnComponentHit.AddDynamic(this, &ACVProjectile::OnHit);

	if (ExplosionDelay > 0.0f) {
		GetWorldTimerManager().SetTimer(TimerHandle_Explode, this, &ACVProjectile::Explode, 1.0f,false, ExplosionDelay);
	}
}

void ACVProjectile::Explode()
{

	//apply explosion impulse force
	FVector BoostIntensity = FVector::UpVector*ExplosionImpulse;
	MeshComp->AddImpulse(BoostIntensity, NAME_None, false);

	RadialForceComp->FireImpulse();

	//UE_LOG(LogTemp, Warning, TEXT("Trying to explode!"));

	// Add ignoredActors if required
	TArray<AActor*> ignoredActors;
	
	FVector MyLocation = this->GetActorLocation();


	//apply radial damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, GetActorLocation(), 250, UDamageType::StaticClass(), ignoredActors, this, nullptr, false, ECollisionChannel::ECC_Visibility);

	//play effects
	DrawDebugSphere(GetWorld(), GetActorLocation(), 250, 12, FColor::Red, false, 2.0f, 0, 1.0f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, MyLocation);

	Destroy();
}

void ACVProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("Trying to overlap!"));

	if (bFleshProjectile)
	{
		EPhysicalSurface SurfaceType = SurfaceType_Default;

		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		if(!SurfaceType)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Can't get surface type!"));

		}
		
		if (SurfaceType == SURFACE_FLESHVULNERABLE || SurfaceType == SURFACE_FLESHDEFAULT)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit some flesh!"));
			Explode();
		}
	}
	
	if (bImpactProjectile) {
		
		if (ImpactNum > 1) {
			ImpactNum--;
		}

		if (ImpactNum == 1) {
			Explode();
		}
	}

	
}

// Called every frame
void ACVProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

