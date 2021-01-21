// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComp.h"

// Sets default values for this component's properties
USHealthComp::USHealthComp()
{
	//PrimaryComponentTick.bCanEverTick = true;
	DefaultHealth = 100;
}


// Called when the game starts
void USHealthComp::BeginPlay()
{
	Super::BeginPlay();
	AActor* myOwner = GetOwner();
	myOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComp::HandleTakeAnyDamage);
	Health = DefaultHealth;
	
}

void USHealthComp::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser ) 
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
}

/*
void USHealthComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

*/

