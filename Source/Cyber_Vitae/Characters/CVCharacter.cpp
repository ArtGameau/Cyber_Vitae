// Fill out your copyright notice in the Description page of Project Settings.


#include "CVCharacter.h"
#include "Cyber_Vitae.h"
#include "Weapons/CVWeapon.h"
#include "Interactive/CVInteractiveActor.h"
#include "Components/CVHealthComponent.h"
#include "Components/CVInventoryComponent.h"
#include "Components/CVWeaponsComponent.h"
#include "Components/InputComponent.h"
#include "Effects/CVBaseEffect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"


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

	ZoomedSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("ZoomedSpringArmComp"));
	ZoomedSpringArmComp->bUsePawnControlRotation = true;
	ZoomedSpringArmComp->SetupAttachment(Cast<USceneComponent>(GetMesh()), "HeadSocket");

	ZoomedCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ZoomedCameraComp"));
	ZoomedCameraComp->SetupAttachment(ZoomedSpringArmComp);
	ZoomedCameraComp->bIsActive = false;

	HealthComp = CreateDefaultSubobject<UCVHealthComponent>(TEXT("HealthComp"));

	InventoryComp = CreateDefaultSubobject<UCVInventoryComponent>(TEXT("InventoryComp"));

	WeaponsComp = CreateDefaultSubobject<UCVWeaponsComponent>(TEXT("WeaponsComp"));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	ZoomedFOV = 60.0f;
	ZoomInterpSpeed = 20;
}

// Called when the game starts or when spawned
void ACVCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = ZoomedCameraComp->FieldOfView;

	/*
	SpawnWeapons();
	*/
	EquippedWeapon = WeaponsComp->FirstWeapon();
	EquippedWeapon->ActivateWeapon();

	bDied = false;
	bWantsToZoom = false;

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

void ACVCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ACVCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ACVCharacter::NextWeapon()
{
	if (WeaponsComp) {
		EquippedWeapon=WeaponsComp->NextWeapon();
	}
}

void ACVCharacter::PreviousWeapon()
{
	if (WeaponsComp) {
		EquippedWeapon = WeaponsComp->PreviousWeapon();
	}
}

void ACVCharacter::Interact()
{
	//interact only if we are focused on interactive actor and that actor is not already in use
	//prevents same pickup actor to be picked up twice when fast button press
	if (CurrentInteractive && !CurrentInteractive->bIsInUse) {
		CurrentInteractive->Interact(this);
	}
}

//check if we are seeing any interactable actors
void ACVCharacter::CheckForInteractables()
{

	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector ViewDirection = CameraComp->GetComponentRotation().Vector();
	FVector TraceEnd = TraceStart + (ViewDirection * 700);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		ACVInteractiveActor* Interactive = Cast<ACVInteractiveActor>(Hit.GetActor());
		if(Interactive)
		{
			if (Interactive != CurrentInteractive) {

				//setting up outline effect
				Interactive->GetMesh()->SetRenderCustomDepth(true);
				Interactive->GetMesh()->SetCustomDepthStencilValue(253);

				//disable outline effect on old focused interactive object
				if (CurrentInteractive) {
					CurrentInteractive->GetMesh()->SetRenderCustomDepth(false);
				}

				CurrentInteractive = Interactive;
				return;
			}
		}
	}

	else if (CurrentInteractive) {
		
		//unable outline effect if we are not looking at object anymore
		CurrentInteractive->GetMesh()->SetRenderCustomDepth(false);

		//if we didn't hit anything current interactive is null pointer
		CurrentInteractive = nullptr;
	}
}

void ACVCharacter::SetZoom()
{
	if (bWantsToZoom && EquippedWeapon->bCanZoom) {
		ZoomedCameraComp->bIsActive = true;
		ZoomedCameraComp->SetFieldOfView(ZoomedFOV);
		CameraComp->bIsActive = false;
	}
	else
	{
		ZoomedCameraComp->bIsActive = false;
		CameraComp->bIsActive = true;
	}
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

void ACVCharacter::UseEffect()
{
	if(CurrentEffectClass)
	{
		CurrentEffect =Cast<ACVBaseEffect> (GetWorld()->SpawnActor(CurrentEffectClass));
		if (CurrentEffect) {
			CurrentEffect->SetOwner(this);
		}
		UE_LOG(LogTemp, Log, TEXT("Effect class spawn!"));

		CurrentEffect->Use();
	}
}

void ACVCharacter::DestroyEffect()
{
	CurrentEffect->Destroy();

	CurrentEffectClass = nullptr;
}

// Called every frame
void ACVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	SetZoom();

	CheckForInteractables();
	
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

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ACVCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ACVCharacter::EndZoom);

	PlayerInputComponent->BindAction("SwitchUp", IE_Pressed, this, &ACVCharacter::NextWeapon);
	PlayerInputComponent->BindAction("SwitchDown", IE_Released, this, &ACVCharacter::PreviousWeapon);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACVCharacter::Interact);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ACVCharacter::Reload);
}

void ACVCharacter::FindAndReload(TSubclassOf<ACVWeapon> WeaponType)
{
	WeaponsComp->FindAndReload(WeaponType);
}

void ACVCharacter::Reload()
{
	if (InventoryComp->Remove(EquippedWeapon->AmmoID)) {
		EquippedWeapon->Reload();
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("You don't have any ammo left!"));
	}
}

void ACVCharacter::StartFire()
{
	EquippedWeapon->StartFire();
}

void ACVCharacter::StopFire()
{
	EquippedWeapon->StopFire();
}


