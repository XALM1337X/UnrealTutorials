// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupPowerBase.generated.h"

/*UENUM()
enum class PowerType : uint8 {
	Speed  UMETA(DisplayName = "Speed"),
	Health UMETA(DisplayName = "Health")
};*/

UCLASS()
class COOPGAME_API APickupPowerBase : public AActor
{
	GENERATED_BODY()
//Methods/Functions	
public:	
	// Sets default values for this actor's properties
	APickupPowerBase();
	
	void InitPower();

	void ActivatePickupPower();

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnPickupPowerTicked();

	UFUNCTION()
	void TickPickupPower();


//Member Variables
public:	


//Methods/Functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

//Member Variables
protected:

	UPROPERTY(EditDefaultsOnly , Category="Powerups")
	float PowerupInterval;  
	
	UPROPERTY(EditDefaultsOnly , Category="Powerups")
	int32 TotalNumberOfTicks; 

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Powerups")
    PowerType PowerUpType;*/

	int32 TicksProcessed;

	FTimerHandle TimeHandle; 


};
