// Fill out your copyright notice in the Description page of Project Settings.


#include "CVHealEffect.h"
#include "Characters/CVCharacter.h"
#include "Components/CVHealthComponent.h"
#include "Engine/World.h"

ACVHealEffect::ACVHealEffect()
{
	Intensity = 30;

}

void ACVHealEffect::Use()
{
	Super::Use();

	UE_LOG(LogTemp, Log, TEXT("Using effect!"));
	
	AActor* ThisOwner = GetOwner();

	ACVCharacter* MyOwner = Cast<ACVCharacter>(ThisOwner);
	
	if(MyOwner)
	{

		UCVHealthComponent* HealthComponent=Cast<UCVHealthComponent>(MyOwner->GetComponentByClass(UCVHealthComponent::StaticClass()));
		
		HealthComponent->Heal(Intensity);

		UE_LOG(LogTemp, Log, TEXT("Heal effect done!"));

		bIsFinished = true;
	}
}
