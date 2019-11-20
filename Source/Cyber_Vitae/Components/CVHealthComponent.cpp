// Fill out your copyright notice in the Description page of Project Settings.


#include "CVHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UCVHealthComponent::UCVHealthComponent()
{
	DefaultHealth = 100;
	
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

	//update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	bIsDead = Health <= 0.0f;

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);

	if (bIsDead) {

		//ACVGameMode* GM = Cast<ACVGameMode>(GetWorld()->GetAuthGameMode());

		//if (GM && DamageCauser != DamagedActor) {

			//GM->OnActorKilled.Broadcast(GetOwner(), DamageCauser, InstigatedBy);

			//UE_LOG(LogTemp, Log, TEXT("Actor is killed!"));
		//}
	}
	
}

float UCVHealthComponent::GetHealth() const
{
	return Health;
}

bool UCVHealthComponent::IsFriendly(AActor* ActorA, AActor* ActorB)
{
	if(ActorA==nullptr || ActorB==nullptr)
	{
		return false;
	}

	UCVHealthComponent* HealthCompA = Cast<UCVHealthComponent>(ActorA->GetComponentByClass(UCVHealthComponent::StaticClass()));
	UCVHealthComponent* HealthCompB = Cast<UCVHealthComponent>(ActorB->GetComponentByClass(UCVHealthComponent::StaticClass()));

	//if it doesn't have health it can't be damaged
	if(HealthCompA==nullptr || HealthCompB==nullptr)
	{
		return true;
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

	OnHealthChanged.Broadcast(this,Health,-HealAmount,nullptr,nullptr,nullptr);
}






