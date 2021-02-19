// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

enum class RunState : uint8 
{
	Walk,
	Run
};
enum class CrouchState : uint8 
{
	Crouch,
	Stand
};

enum class JumpState : uint8 
{
	Jump,
	Fall
};

enum class AimState : uint8 
{
	Zoom,
	Hip
};

enum class FireState : uint8 
{
	Fire,
	Release
};
enum class WeaponState : uint8 
{
	Primary,
	Secondary
};

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
class USHealthComp;
UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

//Public Member Variables
public:
	
//Public Methods/Functions
public:	
	ASCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	bool GetIsAiming();

	bool GetFiringState();

	void SetFiringState(bool);

	ASWeapon* GetWeapon();

//Protected Member Variables
protected:
	UPROPERTY(BlueprintReadWrite)
	ASWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category="Player")
	bool isAiming;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float zoomPOV;

	UPROPERTY(EditDefaultsOnly, Category="Player", meta = ( ClampMin = 0.1, ClampMax = 100.0))
	float zoomInterpolationSpeed;

	bool isCrouching;
	
	float hipPOV;

	bool isFiring;

	FTimerHandle th_time_between_shots;


//Protected Methods/Functions
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ToggleCrouch();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Reload();
	void Fire();

	//UProperties 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComp* HealthComp;
	//These are Toggle Templates bound to action inputs.
	//Sprint
	void ToggleSprint(RunState speed);
	template<RunState speed>
	void ToggleSprint() {ToggleSprint(speed);}

	//Jump
	void ToggleJump(JumpState jumping);	
	template<JumpState jumping>
	void ToggleJump() {ToggleJump(jumping);}

	void ToggleAim(AimState aim);
	template<AimState aim>
	void ToggleAim() {ToggleAim(aim);}

	void ToggleFire(FireState fire);
	template<FireState fire>
	void ToggleFire() {ToggleFire(fire);}


};
