// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComp.generated.h"
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USHealthComp*, HealthComp, float, health, float, healthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

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
	float health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
	float defaultHealth;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Events")
	void HandleTakePointDamage( AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);	
		
};
