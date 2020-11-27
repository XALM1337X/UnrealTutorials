// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"
class UPawnSensingComponent;


UENUM(BlueprintType)
enum class EAIGuardState : uint8
{
	Idle,
	Suspicious,
	Alerted
};


UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()
private:
	int TickCount;
public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnsensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* pawn);

	UFUNCTION()
	void OnNoiseHear(APawn* pawn, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetRotation();

	UFUNCTION(BlueprintImplementableEvent, Category="AI")
	void OnStateChange(EAIGuardState NewState);

	void SetGuardState(EAIGuardState NewState);

	void MoveToNextPoint();

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly,Category="AI", meta=(EditCondition="bPatrol"))
	AActor* FirstPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	AActor* SecondPoint;

	AActor* CurrentPoint;

	FRotator OriginalLookDirection;

	FTimerHandle th_rot_reset;
	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIGuardState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	//
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
