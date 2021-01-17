// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringarmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SWeapon.h"
#include "SWeaponGrenadeLauncher.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	//GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComp);
	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	moveComp->GravityScale = 3.0;
	moveComp->JumpZVelocity = 1000.0f;
	zoomPOV = 65.0f;
	zoomInterpolationSpeed = 20.0f;
	isAiming = false;
	isFiring = false;
	isCrouching = false;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	hipPOV = CameraComponent->FieldOfView;
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float targetPOV = isAiming ? zoomPOV : hipPOV;
	float newPOV = FMath::FInterpTo(CameraComponent->FieldOfView, targetPOV, DeltaTime, zoomInterpolationSpeed);
	CameraComponent->SetFieldOfView(newPOV);
	
}

void ASCharacter::MoveForward(float Value) 
{
	AddMovementInput(GetActorForwardVector() * Value);
	//UE_LOG(LogTemp, Warning, TEXT("VALUE: %f"),Value);
}

void ASCharacter::MoveRight(float Value) 
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::Reload()
{
	//UE_LOG(LogTemp, Warning, TEXT("RELOAD"));
	if (CurrentWeapon->GetRemainingClips() > 0 && CurrentWeapon->GetCurrentAmmoCount() < CurrentWeapon->GetMaxClipSize() && !this->isFiring) 
	{
		CurrentWeapon->ReloadWeapon();
	}
}

void ASCharacter::Fire()
{
	if (CurrentWeapon && CurrentWeapon->GetCurrentAmmoCount() > 0 && !CurrentWeapon->GetReloadState())
	{
		CurrentWeapon->CallFire();
	}
}

void ASCharacter::ToggleCrouch()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController *PC = Cast<APlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{			
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				UCharacterMovementComponent* PawnMovement = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
				if (PawnMovement)
				{
					if (!this->isCrouching)
					{
						Crouch();
						this->isCrouching = true;
					} else 
					{
						UnCrouch();
						this->isCrouching = false;
					}					
				}
			}
		}
	}
}
void ASCharacter::ToggleSprint(RunState speed)
{
	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController *PC = Cast<APlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{			
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				UCharacterMovementComponent* PawnMovement = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
				if (PawnMovement)
				{
					switch(speed)
					{
						case RunState::Walk:
						{
							PawnMovement->MaxWalkSpeed = 600;
							break;
						}
						case RunState::Run:
						{
							PawnMovement->MaxWalkSpeed = 1000;
							break;
						}
					}
				}
			}
		}
	}
}

void ASCharacter::ToggleJump(JumpState jumping)
{
	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController *PC = Cast<APlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			ACharacter* character = PC->GetCharacter();
			if (character)
			{
				switch(jumping)
				{
					case JumpState::Jump:
							character->Jump();
							break;
					case JumpState::Fall:
							break;
				}
			}
		}
	}
}

void ASCharacter::ToggleAim(AimState aim)
{		
	switch(aim)
	{
		case AimState::Zoom:
		{
			isAiming = true;
			break;
		}			
		
		case AimState::Hip:
		{
			isAiming = false;
			break;
		}
	}	
}

void ASCharacter::ToggleFire(FireState fire)
{
	switch(fire)
	{
		case FireState::Fire:
		{
			this->SetFiringState(true);
			if (CurrentWeapon)
			{
				GetWorldTimerManager().SetTimer(th_time_between_shots, this, &ASCharacter::Fire, .12f, true, 0.0f);
			}	
			break;
		}
	
		case FireState::Release:
		{
			this->SetFiringState(false);
			if (CurrentWeapon)
			{
				GetWorldTimerManager().ClearTimer(th_time_between_shots);
			}
			break;
		}
	}
}

bool ASCharacter::GetIsAiming()
{
	return this->isAiming;
} 

bool ASCharacter::GetFiringState()
{
	return this->isFiring;
}

void ASCharacter::SetFiringState(bool value)
{
	this->isFiring = value;
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::ToggleCrouch);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::ToggleCrouch<CrouchState::Stand>);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::ToggleSprint<RunState::Run>);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::ToggleSprint<RunState::Walk>);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::ToggleJump<JumpState::Jump>);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::ToggleJump<JumpState::Fall>);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASCharacter::ToggleAim<AimState::Zoom>);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASCharacter::ToggleAim<AimState::Hip>);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::ToggleFire<FireState::Fire>);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::ToggleFire<FireState::Release>);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASCharacter::Reload);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}