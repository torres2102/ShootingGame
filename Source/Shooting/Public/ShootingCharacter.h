// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShootingCharacter.generated.h"

UCLASS()
class SHOOTING_API AShootingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShootingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float Turn);
	void LookUpAtRate(float Turn);

	void FireWeapon();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent * CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera",meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category= "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category= Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem * BeamParticles;

public:

	FORCEINLINE  USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE  UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
