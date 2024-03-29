// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComp.generated.h"
//NOTE: This Delegate was originally created to demonstrate the ability to create blueprint broadcast events.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FOnHealthChangedSignature, USHealthComp*, HealthComp, float, health, float, healthDelta, class AController*, InstigatedBy, AActor*, DamageCauser, FVector, ShotFromDirection, FName, BoneName );
//UPROPERTY(BlueprintAssignable, Category="Events")
//FOnHealthChangedSignature onHealthChanged;

UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComp : public UActorComponent
{
	GENERATED_BODY()



//Public member variables
public:	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChangedSignature onHealthChanged;

	ENetRole Role;


//Public functions/methods
public:	
	// Sets default values for this component's properties
	USHealthComp();
	UFUNCTION(BlueprintCallable)
	void SetHealth(float val);
	float GetHealth();
	

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
	float health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HealthComponents")
	float defaultHealth;

	bool isDead;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);	
	
	UFUNCTION()
	void HandleTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser ); 
	
	UFUNCTION(Client, Reliable)
	void ClientOnHealthChange(USHealthComp* HealthComp, float current_health, float healthDelta, class AController* InstigatedBy, AActor* DamageCauser, FVector ShotFromDirection, FName BoneName);

	UFUNCTION(NetMulticast, Reliable)
	void ApplyPhysicsPointDamage(ASCharacter* character, FVector ShotFromDirection, FName BoneName);

	UFUNCTION(NetMulticast, Reliable)
	void ApplyPhysicsRadialDamage(ASCharacter* character, FVector ExplosionOrigin, float SphereRadius, float ExplosionForce);

	void CleanUp();

};
