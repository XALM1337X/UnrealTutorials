// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComp.h"


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
	//myOwner->OnTakePointDamage.AddDynamic(this, &USHealthComp::HandleTakePointDamage);
	
}

/*void USHealthComp::HandleTakePointDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser ) 
{
	if (Damage <= 0.0f)
	{
		return;
	}
	//NOTE: If moving damage from blueprint back here to c++ uncomment this, and delete any blueprint that effects health component.
	//this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);
	//UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(health));

	//Using Broadcast to trigger blueprint
	onHealthChanged.Broadcast(this, this->health, Damage, DamageType, InstigatedBy, DamageCauser);
}*/

/*
void USHealthComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}
*/

