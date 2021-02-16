// Fill out your copyright notice in the Description page of Project Settings.
#include "SHealthComp.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values for this component's properties
USHealthComp::USHealthComp()
{
	//PrimaryComponentTick.bCanEverTick = true;
	defaultHealth = 100;
	this->health=0;
}


// Called when the game starts
void USHealthComp::BeginPlay()
{
	Super::BeginPlay();
	AActor* myOwner = GetOwner();
	this->health = this->defaultHealth;
	myOwner->OnTakePointDamage.AddDynamic(this, &USHealthComp::HandleTakePointDamage);
	//myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComp::HandleAnyDamage);

	
}

/*	NOTE:This function is purely for testing with the pain volume, It will overtake and other dynamically overriden event functions related to 
	Actors that can take damage or have the SHealthComp.

void USHealthComp::HandleAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) 
{
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);
	if (this->health <= 0.0f) {
		UPawnMovementComponent* MC = Cast<ASCharacter>(DamagedActor)->GetMovementComponent();
		if (MC) {
			MC->StopMovementImmediately();
		} else {
			UE_LOG(LogTemp, Log, TEXT("FAIL"));		
		}
		
	}
	UE_LOG(LogTemp, Log, TEXT("CurrentHealth: %f"), this->health);
}*/

void USHealthComp::HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) 
{
	if (Damage <= 0.0f)
	{		
		return;
	}
	//NOTE: If moving damage from blueprint back here to c++ uncomment this, and delete any blueprint that effects health component.
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);

	if (this->health <= 0.0f) {
		UPawnMovementComponent* MC = Cast<ASCharacter>(DamagedActor)->GetMovementComponent();
		if (MC) {
			MC->StopMovementImmediately();
			//UE_LOG(LogTemp, Log, TEXT("TRIGGER_1"));
		} else {
			//UE_LOG(LogTemp, Log, TEXT("TRIGGER_2"));
		}
		ASCharacter* character = Cast<ASCharacter>(DamagedActor);
		if (character) {
			USkeletalMeshComponent* mesh = character->GetMesh();
			if (mesh) {
				mesh->SetSimulatePhysics(true);
				mesh->AddImpulseAtLocation(ShotFromDirection*8000, ShotFromDirection ,BoneName);   //TODO: Remove magic number later with adjustable UPROPERTY.
				FTimerHandle Handler;
				GetWorld()->GetTimerManager().SetTimer(Handler, this,&USHealthComp::CleanUp, 3.0f, false);
				//UE_LOG(LogTemp, Log, TEXT("TRIGGER"));
			}
		}
		
	}

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(health));

	//Using Broadcast to trigger blueprint
	//onHealthChanged.Broadcast(this, this->health, Damage, DamageType, InstigatedBy,  DamageCauser, ShotFromDirection, BoneName);
}

void USHealthComp::CleanUp() {
	AActor* myOwner = GetOwner();
	myOwner->Destroy();
}
