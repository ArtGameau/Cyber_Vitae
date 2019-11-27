// Fill out your copyright notice in the Description page of Project Settings.


#include "CVCharacter.h"
#include "Cyber_Vitae.h"
#include "Weapons/CVWeapon.h"
#include "Interactive/CVInteractiveActor.h"
#include "Components/CVHealthComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
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
	ZoomedSpringArmComp->SetupAttachment(Cast<USceneComponent>(GetMesh()),"HeadSocket");

	ZoomedCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ZoomedCameraComp"));
	ZoomedCameraComp->SetupAttachment(ZoomedSpringArmComp);
	ZoomedCameraComp->bIsActive=false;

	HealthComp = CreateDefaultSubobject<UCVHealthComponent>(TEXT("HealthComp"));
	
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	WeaponAttachSocketName = "WeaponSocket";

	ZoomedFOV =25.0f;
	ZoomInterpSpeed = 20;
	
	CurrentWeaponPlace = 0;
	WeaponStackSize = 4;

	EquipedWeaponClasses.SetNum(WeaponStackSize);
}

// Called when the game starts or when spawned
void ACVCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = ZoomedCameraComp->FieldOfView;
	
	SpawnWeapon();

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
	CurrentWeaponPlace=(CurrentWeaponPlace + 1 ) % WeaponStackSize;
	EquippedWeapon->Destroy();

	SpawnWeapon();
}

void ACVCharacter::PreviousWeapon()
{
	CurrentWeaponPlace = (CurrentWeaponPlace - 1 + WeaponStackSize) % WeaponStackSize;
	EquippedWeapon->Destroy();

	SpawnWeapon();	
}

void ACVCharacter::Interact()
{
	if (CurrentInteractive) {
		CurrentInteractive->Interact(this);
	}
}


void ACVCharacter::SpawnWeapon()
{
	//Spawn a next weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EquippedWeapon = GetWorld()->SpawnActor<ACVWeapon>(EquipedWeaponClasses[CurrentWeaponPlace], FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (EquippedWeapon) {
		EquippedWeapon->SetOwner(this);
		EquippedWeapon->AttachToComponent(Cast<USceneComponent>(GetMesh()), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
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
			CurrentInteractive = Interactive;
			return;
		}
	}

	//if we didn't hit anything current interactive is null pointer
	CurrentInteractive = nullptr;
	
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
}

void ACVCharacter::StartFire()
{
	EquippedWeapon->StartFire();
}

void ACVCharacter::StopFire()
{
	EquippedWeapon->StopFire();
}



