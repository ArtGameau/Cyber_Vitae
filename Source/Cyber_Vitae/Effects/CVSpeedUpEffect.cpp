// Fill out your copyright notice in the Description page of Project Settings.


#include "CVSpeedUpEffect.h"
#include "Characters/CVCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

ACVSpeedUpEffect::ACVSpeedUpEffect() {
	Boost = 200.0f;

	Duration = 10.0f;
}

void ACVSpeedUpEffect::Use()
{
	ACVCharacter* MyOwner = Cast<ACVCharacter>(GetOwner());

	if (MyOwner) {

		
		UCharacterMovementComponent* Movement=Cast<UCharacterMovementComponent>(MyOwner->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
		
		if (Movement) {
			float CurrentSpeed = Movement->MaxWalkSpeed;
			Movement->MaxWalkSpeed = CurrentSpeed + Boost;
			UE_LOG(LogTemp, Log, TEXT("Speed up started!"));
		}

		if (Duration > 0.0f) {
			GetWorldTimerManager().SetTimer(TimerHandle_EffectDuration, this, &ACVSpeedUpEffect::EndEffect, Duration, false, Duration);
		}
	}
}

void ACVSpeedUpEffect::EndEffect()
{
	UE_LOG(LogTemp, Log, TEXT("Speed up ended!"));

	GetWorldTimerManager().ClearTimer(TimerHandle_EffectDuration);

	ACVCharacter* MyOwner = Cast<ACVCharacter>(GetOwner());

	if (MyOwner) {
		UCharacterMovementComponent* Movement = Cast<UCharacterMovementComponent>(MyOwner->GetComponentByClass(UCharacterMovementComponent::StaticClass()));

		if (Movement) {
			float CurrentSpeed = Movement->MaxWalkSpeed;
			Movement->MaxWalkSpeed = CurrentSpeed - Boost;
		}
		MyOwner->DestroyEffect();
	}

}
