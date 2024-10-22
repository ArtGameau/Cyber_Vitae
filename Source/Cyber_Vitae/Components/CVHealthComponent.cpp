// Fill out your copyright notice in the Description page of Project Settings.


#include "CVHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UCVHealthComponent::UCVHealthComponent()
{
	DefaultHealth = 100;

	DefaultArmor = 100;

	bHasArmor = false;
	
	bIsDead = false;

	TeamNum = 255;
}

void UCVHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UCVHealthComponent::HandleTakeAnyDamage);
	}

	Health = DefaultHealth;
	bIsDead = false;
	
	UE_LOG(LogTemp, Log, TEXT("New alive actor arrived at game! Team number: %s"), *FString::SanitizeFloat(TeamNum));

}


void UCVHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	//if damage is zero of if actor is dead no need for calculating health
	if(Damage<=0 || bIsDead)
	{
		return;
	}

	//self-caused damage and friendly damage are both impossible
	if(DamageCauser!=DamagedActor && IsFriendly(DamagedActor, DamageCauser))
	{
		return;
	}

	if(bHasArmor && Armor>0.0)
	{
		//if actor took more damage than armor left then remaining damage will be taken from health
		if (Damage >= Armor) {
			Damage = Damage - Armor;
			Armor = 0.0;
			Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
			UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
		}
		else
		{
			Armor = FMath::Clamp(Armor - Damage, 0.0f, DefaultArmor);
			UE_LOG(LogTemp, Log, TEXT("Armor Changed: %s"), *FString::SanitizeFloat(Armor));
		}
	}
	else {
		//update health clamped
		Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
		
		UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
	}

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead) {

		//ACVGameMode* GM = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());

		if (DamageCauser != DamagedActor) {

			//GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);

			UE_LOG(LogTemp, Log, TEXT("Actor is killed!"));
		}
	}
	
}

float UCVHealthComponent::GetHealth() const
{
	return Health;
}

void UCVHealthComponent::SetDefaultHealth(float NewHealth)
{
	DefaultHealth = NewHealth;
}

void UCVHealthComponent::SetArmor()
{
	bHasArmor = true;
	Armor = DefaultArmor;
}

void UCVHealthComponent::ResetArmor()
{
	bHasArmor = false;
	Armor = 0;
}

bool UCVHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if(ActorA==nullptr || ActorB==nullptr)
	{
		return false;
	}

	UCVHealthComponent* HealthCompA = Cast<UCVHealthComponent>(ActorA->GetComponentByClass(UCVHealthComponent::StaticClass()));
	UCVHealthComponent* HealthCompB = Cast<UCVHealthComponent>(ActorB->GetComponentByClass(UCVHealthComponent::StaticClass()));

	//can receive damage from grenades
	if(HealthCompA==nullptr || HealthCompB==nullptr)
	{
		return false;
	}

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}

void UCVHealthComponent::Heal(float HealAmount)
{
	if(HealAmount<=0.0f || Health<=0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health+HealAmount, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));


	OnHealthChanged.Broadcast(this,Health,-HealAmount,nullptr,nullptr,nullptr);
}






