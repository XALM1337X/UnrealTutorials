// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringarmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("VALUE: %f"),);
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

void ASCharacter::ToggleCrouch(CrouchState posture)
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
					switch(posture)
					{
						case CrouchState::Crouch:
							Crouch();
							break;

						case CrouchState::Stand:
							UnCrouch();
							break;
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
							UE_LOG(LogTemp, Warning, TEXT("Walking"));
							break;
						}
						case RunState::Run:
						{
							PawnMovement->MaxWalkSpeed = 1000;
							UE_LOG(LogTemp, Warning, TEXT("Running"));
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

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this, &ASCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::ToggleCrouch<CrouchState::Crouch>);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::ToggleCrouch<CrouchState::Stand>);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::ToggleSprint<RunState::Run>);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::ToggleSprint<RunState::Walk>);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::ToggleJump<JumpState::Jump>);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::ToggleJump<JumpState::Fall>);
}

