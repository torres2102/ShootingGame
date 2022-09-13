// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "ShootingCharacter.h"

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr) {
		ShooterCharacter = Cast<AShootingCharacter>(TryGetPawnOwner());

	}
	if (ShooterCharacter) {
		FVector Velocity = { ShooterCharacter->GetVelocity()};
		Velocity.Z = 0.f;
		speed = Velocity.Size();

		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
			bIsAccelerating = true;
		else
			bIsAccelerating = false;
	}
}


void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShootingCharacter>(TryGetPawnOwner());
}
