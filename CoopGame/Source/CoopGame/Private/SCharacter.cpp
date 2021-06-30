// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringarmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SWeapon.h"
#include "SWeaponGrenadeLauncher.h"
#include "Components/CapsuleComponent.h"
#include "CoopGame/CoopGame.h"
#include "SHealthComp.h"
#include "Net/UnrealNetwork.h"
static int32 DebugMode = 0;
FAutoConsoleVariableRef DebugDeath(TEXT("COOP.DebugDeath"), DebugMode, TEXT("Set death defaults"), ECVF_Cheat);
// Sets default values
ASCharacter::ASCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	//GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<USHealthComp>(TEXT("HealthComp"));
	

	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	moveComp->GravityScale = 3.0;
	moveComp->JumpZVelocity = 1000.0f;
	zoomPOV = 65.0f;
	zoomInterpolationSpeed = 20.0f;
	isAiming = false;
	isFiring = false;
	isCrouching = false;
	isSprinting = false;
	WeaponAttachSocketName = "WeaponSocket";


	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel()
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay() {
	Super::BeginPlay();
	hipPOV = CameraComponent->FieldOfView;
	if (GetLocalRole() == ROLE_Authority) {
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(starterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, params);
		
		StoredLauncher = GetWorld()->SpawnActor<ASWeaponGrenadeLauncher>(launcherWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, params);
		if (StoredLauncher) {
			StoredLauncher->SetOwner(this);
			//StoredLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
			StoredLauncher->SetActorHiddenInGame(true);
			StoredLauncher->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			StoredLauncher->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
		
		if (CurrentWeapon) {
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	float targetPOV = isAiming ? zoomPOV : hipPOV;
	float newPOV = FMath::FInterpTo(CameraComponent->FieldOfView, targetPOV, DeltaTime, zoomInterpolationSpeed);
	CameraComponent->SetFieldOfView(newPOV);
	
	
	if (GetLocalRole() == ROLE_Authority) {
		if (DebugMode > 0) {
			HealthComp->SetHealth(HealthComp->GetHealth() - .5f);
			HealthComp->onHealthChanged.Broadcast(HealthComp, HealthComp->GetHealth()+33, HealthComp->GetHealth() -1, GetController(),  this, FVector(1.0,1.0,1.0), "Head");
			if (HealthComp->GetHealth() <= 0) {
				//UE_LOG(LogTemp, Warning, TEXT("DEBUG_DEAD"));
				UPawnMovementComponent* MC = GetMovementComponent();
				if (MC) {
					MC->StopMovementImmediately();
				} 
				APlayerController* cont = Cast<APlayerController>(this->GetController());
				if (cont) {
					this->DisableInput(cont);
				}
				USkeletalMeshComponent* mesh = GetMesh();
				if (mesh) {
					mesh->SetSimulatePhysics(true);
				}
			}
		}

	}	
}

void ASCharacter::MoveForward(float Value) {
	if(this->isAiming) {
		AddMovementInput(GetActorForwardVector() * Value/2);
	} else {
		AddMovementInput(GetActorForwardVector() * Value);
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("VALUE: %f"),Value);
}

void ASCharacter::MoveRight(float Value) {

	if(this->isAiming) {
		AddMovementInput(GetActorRightVector() * Value/2);
	} else {
		AddMovementInput(GetActorRightVector() * Value);
	}
}

void ASCharacter::Reload() {
	//UE_LOG(LogTemp, Warning, TEXT("RELOAD"));
	CurrentWeapon->ReloadWeapon();	
}

void ASCharacter::Fire() {
	//UE_LOG(LogTemp, Warning, TEXT("ASCharacter::FIRE_TRIGGER_0"));
	CurrentWeapon->Fire();

}

void ASCharacter::ServerSwitchToPrimary_Implementation() {
	this->SwitchToPrimary();
}

void ASCharacter::SwitchToPrimary() { 
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerSwitchToPrimary();
		return;
	}
	if (CurrentWeapon) {
		//Check Type of weapon.
		if (CurrentWeapon->IsA(ASWeaponGrenadeLauncher::StaticClass())) {
			if (StoredPrimary) {
				//TODO: Add Server RPC's to ASWeapon and  ASWeaponGrenadeLauncher.
				//Everything called in this block needs to be replicated
				
				CurrentWeapon->SetActorHiddenInGame(true);
				//CurrentWeapon->GetWeaponMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				CurrentWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				StoredLauncher = CurrentWeapon;
				CurrentWeapon = StoredPrimary;
				StoredPrimary = nullptr;
				CurrentWeapon->SetActorHiddenInGame(false);
				//CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
				CurrentWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				CurrentWeapon->ClientOnAmmoChanged(this, CurrentWeapon->GetCurrentAmmoCount(), CurrentWeapon->GetRemainingClips(), CurrentWeapon->GetMaxClipSize(), CurrentWeapon->weaponName);
				
			} else {
				UE_LOG(LogTemp, Warning, TEXT("No Stored Primary Weapon."));
			}
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Primary::CurrentWeapon Is Null."));
	}
}

void ASCharacter::ServerSwitchToSecondary_Implementation() {
	this->SwitchToSecondary();
}

void ASCharacter::SwitchToSecondary() {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerSwitchToSecondary();
		return;
	}
	if (CurrentWeapon) {
		if (CurrentWeapon->IsA(ASWeapon::StaticClass())) {
			if (StoredLauncher) {
				//TODO: Add Server RPC's to ASWeapon and  ASWeaponGrenadeLauncher.
				//Everything called in this block needs to be replicated
				
				CurrentWeapon->SetActorHiddenInGame(true);
				//CurrentWeapon->GetWeaponMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				CurrentWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				StoredPrimary = CurrentWeapon;
				CurrentWeapon = StoredLauncher;
				StoredLauncher = nullptr;
				CurrentWeapon->SetActorHiddenInGame(false);
				//CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
				CurrentWeapon->GetWeaponMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				CurrentWeapon->ClientOnAmmoChanged(this, CurrentWeapon->GetCurrentAmmoCount(), CurrentWeapon->GetRemainingClips(), CurrentWeapon->GetMaxClipSize(), CurrentWeapon->weaponName);
				
			} else {
				UE_LOG(LogTemp, Warning, TEXT("No Stored Launcher Weapon."))
			}
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Launcher::CurrentWeapon Is Null."));
	}
}

void ASCharacter::ToggleSwitchWeapon(WeaponSelectedState weapon) {
	switch (weapon) {
		case WeaponSelectedState::Primary: {
			this->SwitchToPrimary();
			break;
		}
		case WeaponSelectedState::Secondary: {
			this->SwitchToSecondary();
			break;
		}
	}
}

bool ASCharacter::GetIsAiming() {
	return this->isAiming;
} 

bool ASCharacter::GetFiringState() {
	return this->isFiring;
}

void ASCharacter::ServerSetFiringState_Implementation(bool value) {
	this->SetFiringState(value);
}

void ASCharacter::SetFiringState(bool value) {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerSetFiringState(value);
		return;
	}
	this->isFiring = value;
}

void ASCharacter::ServerSetIsAiming_Implementation(bool value) {
	this->SetIsAiming(value);
}

void ASCharacter::SetIsAiming(bool value) {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerSetIsAiming(value);
		return;
	}
	this->isAiming = value;
}

ASWeapon* ASCharacter::GetWeapon() {
	return this->CurrentWeapon;
}

void ASCharacter::ToggleCrouch() {
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController *PC = Cast<APlayerController>(It->Get());
		if (PC && PC->IsLocalController()) {			
			APawn* Pawn = PC->GetPawn();
			if (Pawn) {
				UCharacterMovementComponent* PawnMovement = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());
				if (PawnMovement) {
					if (!this->isCrouching) {
						Crouch();
						this->isCrouching = true;
					} else {
						UnCrouch();
						this->isCrouching = false;
					}					
				}
			}
		}
	}
}

void ASCharacter::ToggleSprint(RunState speed) {

	switch(speed) {
		case RunState::Walk: {
			this->SprintOff();
			break;
		}
		case RunState::Run:	{
			
			this->SprintOn();
			break;
		}
	}
}

void ASCharacter::ServerToggleSprintOn_Implementation() {
	this->SprintOn();
}

void ASCharacter::SprintOn() {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerToggleSprintOn(); 
		return;
	}	
	this->run_replicator.sprintMsg = true;
	CastRunSpeed(850);
	
}



void ASCharacter::ServerToggleSprintOff_Implementation() {
	this->SprintOff();
}

void ASCharacter::SprintOff() {
	if (GetLocalRole() != ROLE_Authority) {
		this->ServerToggleSprintOff(); 
		return;
	}
	this->run_replicator.sprintMsg = false;
	CastRunSpeed(600);
}


void ASCharacter::CastRunSpeed_Implementation(int speed) {
	GetCharacterMovement()->MaxWalkSpeed = speed;	
}

void ASCharacter::ToggleJump(JumpState jumping) {
	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController *PC = Cast<APlayerController>(It->Get());
		if (PC && PC->IsLocalController()) {
			ACharacter* character = PC->GetCharacter();
			if (character) {
				switch(jumping)	{
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

void ASCharacter::ToggleAim(AimState aim) {		
	switch(aim)	{
		case AimState::Zoom: {
			this->SetIsAiming(true);
			break;
		}			
		
		case AimState::Hip:	{
			this->SetIsAiming(false);
			break;
		}
	}	
}

void ASCharacter::ToggleFire(FireState fire) {
	switch(fire) {
		case FireState::Fire: {
			this->SetFiringState(true);
			if (CurrentWeapon) {
				float FirstDelay = FMath::Max(CurrentWeapon->GetLastFireTime() + CurrentWeapon->GetTimeBetweenShots() - GetWorld()->TimeSeconds, 0.0f);
				GetWorldTimerManager().SetTimer(th_time_between_shots, this, &ASCharacter::Fire, CurrentWeapon->GetTimeBetweenShots() , true, FirstDelay);
			}	
			break;
		}

		case FireState::Release: {
			this->SetFiringState(false);
			if (CurrentWeapon) {
				GetWorldTimerManager().ClearTimer(th_time_between_shots);
			}
			break;
		}
	}	
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::ToggleSprint<RunState::Run>);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::ToggleSprint<RunState::Walk>);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::ToggleJump<JumpState::Jump>);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::ToggleJump<JumpState::Fall>);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ASCharacter::ToggleAim<AimState::Zoom>);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ASCharacter::ToggleAim<AimState::Hip>);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::ToggleFire<FireState::Fire>);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::ToggleFire<FireState::Release>);
	PlayerInputComponent->BindAction("SwitchPrimary", IE_Pressed, this, &ASCharacter::ToggleSwitchWeapon<WeaponSelectedState::Primary>);
	PlayerInputComponent->BindAction("SwitchSecondary", IE_Pressed, this, &ASCharacter::ToggleSwitchWeapon<WeaponSelectedState::Secondary>);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASCharacter::Reload);

}

FVector ASCharacter::GetPawnViewLocation() const {
	if (CameraComponent) {
		return CameraComponent->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASCharacter::OnRep_RunStateChange() {
	this->isSprinting = this->run_replicator.sprintMsg;
}



void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASCharacter, CurrentWeapon);
	DOREPLIFETIME(ASCharacter, CameraComponent);
	DOREPLIFETIME(ASCharacter, isAiming);
	DOREPLIFETIME(ASCharacter, run_replicator);
}