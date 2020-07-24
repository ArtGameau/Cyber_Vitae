// Fill out your copyright notice in the Description page of Project Settings.


#include "CVWeapon.h"
#include "CVGameMode.h"
#include "Engine/World.h"
#include "Characters/CVCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Cyber_Vitae.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("CV.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines For Weapons"),
	ECVF_Cheat);


// Sets default values
ACVWeapon::ACVWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";

	bCanZoom = false;

	BonusDamage = 0;

	Range = 1000;

	MagazineSize = 50;

	Name = FText::FromString("No name");
	Description = FText::FromString("Please enter description for this item.");
}

// Called when the game starts or when spawned
void ACVWeapon::BeginPlay()
{
	Super::BeginPlay();

	//find weapon type information in data table and initialise members of this class
	ACVGameMode* GameMode = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* WeaponTable = GameMode->GetWeaponDB();

	if (WeaponTable)
	{
		FWeaponItem* WeaponInfo = WeaponTable->FindRow<FWeaponItem>(WeaponID, "");

		if (WeaponInfo) {

			UE_LOG(LogTemp, Log, TEXT("Initializing weapon from data table!"));

			BaseDamage = WeaponInfo->Damage;
			BonusDamage = WeaponInfo->MaxDamageBonus;
			Range = WeaponInfo->Range;
			MagazineSize = WeaponInfo->Ammo;
			RateOfFire = WeaponInfo->RateOfFire;
			BulletSpread = WeaponInfo->BulletSpread;

			Name = WeaponInfo->Name;
			Thumbnail = WeaponInfo->Thumbnail;
			Description = WeaponInfo->Description;
		}
	}

	TimeBetweenShots = 60 / RateOfFire;

	CurrentAmmo = MagazineSize;

	SetActorEnableCollision(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(false);
}


void ACVWeapon::Fire()
{
	//Trace the world from pawn eyes to crosshair location (center of screen)
	AActor* MyOwner = GetOwner();

	//fire if weapon has owner and has some ammunition, otherwise do nothing 
	if (MyOwner && CurrentAmmo>0) {
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		//bullet spread
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyeLocation + (ShotDirection * Range);

		//Impact effect 'target' parameter
		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams)) {
			//Blocking hit process damage

			AActor* HitActor = Hit.GetActor();
			ACVCharacter* CVOwner = Cast<ACVCharacter>(MyOwner);

			float ActualDamage = BaseDamage;
		
			//setting bonus damage based on owner class
			if (MyOwner) {
				ECharClassEnum OwnerClass = CVOwner->GetCharClass();

				switch (OwnerClass) {
				case ECharClassEnum::CE_Tank:
					ActualDamage = BaseDamage + (Range - Hit.Distance) / Range * BonusDamage;
					break;
				case ECharClassEnum::CE_Hacker:
					ActualDamage = BaseDamage + Hit.Distance / Range * BonusDamage;
					break;
				case ECharClassEnum::CE_Jumper:
					if ((EyeLocation.Z - Hit.Location.Z) > 0) {
						ActualDamage = BaseDamage + (EyeLocation.Z - Hit.Location.Z) / 200 * BonusDamage;
					}
					break;
				default:
					ActualDamage = BaseDamage;
					break;

				}
			}
			

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			if (SurfaceType == SURFACE_FLESHVULNERABLE)
			{
				ActualDamage *= 2.0f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);

			PlayImpactEffect(SurfaceType, Hit.ImpactPoint);

			TracerEndPoint = Hit.ImpactPoint;
		}

		if (DebugWeaponDrawing > 0) {
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		}

		PlayFireEffect(TracerEndPoint);

		LastFireTime = GetWorld()->TimeSeconds;

		CurrentAmmo--;
	}
	
}

void ACVWeapon::PlayFireEffect(FVector EndPoint)
{
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect) {
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);


		if (TracerComp) {
			TracerComp->SetVectorParameter(TracerTargetName, EndPoint);
		}
	}

}

void ACVWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = DefaultImpactEffect;

	switch (SurfaceType)
	{
	case SURFACE_FLESHDEFAULT:
		SelectedEffect = FleshImpactEffect;
		break;
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect) {

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void ACVWeapon::SetBonusDamage(float NewBonus)
{
	BonusDamage = NewBonus;
}

void ACVWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ACVWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ACVWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ACVWeapon::ActivateWeapon()
{
	MeshComp->SetVisibility(true);
	SetActorEnableCollision(ECollisionEnabled::QueryAndPhysics);
}

void ACVWeapon::DeactivateWeapon()
{
	MeshComp->SetVisibility(false);
	SetActorEnableCollision(ECollisionEnabled::NoCollision);
}

void ACVWeapon::Reload()
{
	CurrentAmmo=MagazineSize;
}

