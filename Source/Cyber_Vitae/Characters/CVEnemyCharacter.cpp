// Fill out your copyright notice in the Description page of Project Settings.


#include "CVEnemyCharacter.h"
#include "Weapons/CVWeapon.h"
#include "Components/CVHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"



// Sets default values
ACVEnemyCharacter::ACVEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UCVHealthComponent>(TEXT("HealthComp"));

	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ACVEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bDied = false;

	HealthComp->OnHealthChanged.AddDynamic(this, &ACVEnemyCharacter::OnHealthChanged);

	SpawnWeapon();
}

void ACVEnemyCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ACVEnemyCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ACVEnemyCharacter::BeginCrouch()
{
	Crouch();
}

void ACVEnemyCharacter::EndCrouch()
{
	UnCrouch();
}


void ACVEnemyCharacter::Reload()
{
	if (EquippedWeapon) {
		EquippedWeapon->Reload();
	}
}

void ACVEnemyCharacter::StartFire()
{
	if (EquippedWeapon) {
		EquippedWeapon->StartFire();
	}
}

void ACVEnemyCharacter::StopFire()
{
	if (EquippedWeapon) {
		EquippedWeapon->StopFire();
	}
}

void ACVEnemyCharacter::SpawnWeapon()
{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		EquippedWeapon = GetWorld()->SpawnActor<ACVWeapon>(EquippedWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (EquippedWeapon) {

			UE_LOG(LogTemp, Log, TEXT("Enemy weapon spawned!"));

				EquippedWeapon->SetOwner(this);
				EquippedWeapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
				EquippedWeapon->ActivateWeapon();
		}
}

void ACVEnemyCharacter::OnHealthChanged(UCVHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Health changed!"));

	if (Health <= 0.0f && !bDied)
	{
		//Die!

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bDied = true;

		UE_LOG(LogTemp, Log, TEXT("Actor is killed!"));

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);

		//only if some weapon is equipped set it's destroy time and no collision
		if(EquippedWeapon)
		{
			EquippedWeapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
			EquippedWeapon->SetLifeSpan(10.0f);
		}
	}
}

// Called every frame
void ACVEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACVEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACVEnemyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACVEnemyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACVEnemyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ACVEnemyCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACVEnemyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACVEnemyCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACVEnemyCharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACVEnemyCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACVEnemyCharacter::StopFire);

}

