// Fill out your copyright notice in the Description page of Project Settings.


#include "CVCharacter.h"
#include "Cyber_Vitae.h"
#include "Components/CVHealthComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapons/CVWeapon.h"
#include "Engine/World.h"


// Sets default values
ACVCharacter::ACVCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UCVHealthComponent>(TEXT("HealthComp"));
	
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	WeaponAttachSocketName = "WeaponSocket";

}

// Called when the game starts or when spawned
void ACVCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn a default weapon
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		EquippedWeapon = GetWorld()->SpawnActor<ACVWeapon>(EquipedWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (EquippedWeapon) {
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}

		bDied = false;

		HealthComp->OnHealthChanged.AddDynamic(this, &ACVCharacter::OnHealthChanged);
}

void ACVCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ACVCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ACVCharacter::BeginCrouch()
{
	Crouch();
}

void ACVCharacter::EndCrouch()
{
	UnCrouch();
}


void ACVCharacter::OnHealthChanged(UCVHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	UE_LOG(LogTemp, Log, TEXT("Health changed!"));
	
	if(Health<=0.0f && !bDied)
	{
		//Die!

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EquippedWeapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
		bDied = true;

		UE_LOG(LogTemp, Log, TEXT("Actor is killed!"));

		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
		EquippedWeapon->SetLifeSpan(10.0f);
	}
}

// Called every frame
void ACVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACVCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACVCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACVCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACVCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ACVCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACVCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACVCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACVCharacter::Jump);
	
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACVCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACVCharacter::StopFire);


}

void ACVCharacter::StartFire()
{
	EquippedWeapon->StartFire();
}

void ACVCharacter::StopFire()
{
	EquippedWeapon->StopFire();
}



