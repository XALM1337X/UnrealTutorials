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
	this->health = 0;
}


// Called when the game starts
void USHealthComp::BeginPlay()
{
	Super::BeginPlay();
	AActor* myOwner = GetOwner();
	this->health = this->defaultHealth;
	myOwner->OnTakePointDamage.AddDynamic(this, &USHealthComp::HandleTakePointDamage);	
}

void USHealthComp::HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) 
{
	if (Damage <= 0.0f)
	{		
		return;
	}
	this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);

	if (this->health <= 0.0f) {
		UPawnMovementComponent* MC = Cast<ASCharacter>(DamagedActor)->GetMovementComponent();
		if (MC) {
			MC->StopMovementImmediately();
		} 
		ASCharacter* character = Cast<ASCharacter>(DamagedActor);
		if (character) {
			USkeletalMeshComponent* mesh = character->GetMesh();
			if (mesh) {
				mesh->SetSimulatePhysics(true);
				mesh->AddImpulseAtLocation(ShotFromDirection*8000, ShotFromDirection ,BoneName);   //TODO: Remove magic number later with adjustable UPROPERTY.
				FTimerHandle Handler;
				GetWorld()->GetTimerManager().SetTimer(Handler, this,&USHealthComp::CleanUp, 3.0f, false);
			}
		}
		
	}

	//UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(health));

	//Using Broadcast to trigger blueprint
	//onHealthChanged.Broadcast(this, this->health, Damage, DamageType, InstigatedBy,  DamageCauser, ShotFromDirection, BoneName);
}

void USHealthComp::CleanUp() {
	AActor* myOwner = GetOwner();
	myOwner->Destroy();
}
