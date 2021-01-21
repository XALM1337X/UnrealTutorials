// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComp.generated.h"


UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComp : public UActorComponent
{
	GENERATED_BODY()



//Public member variables
public:	


//Public functions/methods
public:	
	// Sets default values for this component's properties
	USHealthComp();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
	float DefaultHealth;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser );
		
};
