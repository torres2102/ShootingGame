// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AShootingCharacter::AShootingCharacter() : 
	BaseLookUpRate {45.f},
	BaseTurnRate{45.f}

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	 
	//This is for The SpringArmComponent
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;  // لو عملتها ترو بيتحرك مع الكاركتر  
	CameraBoom->SocketOffset = FVector{ 0.0f,50.f,50.f };
	//This is for the follow CameraComponent
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // لو ترو بيتحرك نسبتا الي الروت بتاعها لو فولس بيتحرك مع الروتيشن بتاع الكاركتر  

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AShootingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShootingCharacter::MoveForward(float value)
{
	if(Controller != nullptr && value != 0.0f){
		
		const FRotator Rotation {Controller->GetControlRotation()};
		const FRotator YawRotation {0,Rotation.Yaw,0};
		const FVector Direction {FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};

		AddMovementInput(Direction , value);
		
	}
}

void AShootingCharacter::MoveRight(float value)
{

	if (Controller != nullptr && value != 0.0f) {

		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0,Rotation.Yaw,0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		AddMovementInput(Direction, value);

	}
}

void AShootingCharacter::TurnAtRate(float Turn)
{
	AddControllerYawInput(BaseTurnRate * Turn * GetWorld()->DeltaTimeSeconds);
}

void AShootingCharacter::LookUpAtRate(float Turn)
{
	AddControllerPitchInput(BaseLookUpRate *Turn*GetWorld()->DeltaTimeSeconds);
}

void AShootingCharacter::FireWeapon()
{
	if (FireSound) {
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");

	if (BarrelSocket) {
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
		if (MuzzleFlash) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
		FHitResult FireHit;
		FVector Start = { SocketTransform.GetLocation()};
		FQuat Rotation = { SocketTransform.GetRotation() };
		FVector RotationAxis = { Rotation.GetAxisX() };
		FVector End = { Start + RotationAxis* 50000.f };

		FVector BeamEndPoint = { End };

		GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

		if (FireHit.bBlockingHit) {

			BeamEndPoint = FireHit.Location;

			if (ImpactParticles) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.Location);
			}

		}
		if (BeamParticles) {
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);
			if (Beam) {
				Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
			}
		}
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HipFireMontage) {

		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));


	}
	
}

// Called every frame
void AShootingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShootingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this,&AShootingCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootingCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShootingCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShootingCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AShootingCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AShootingCharacter::Jump);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &AShootingCharacter::FireWeapon);


}

