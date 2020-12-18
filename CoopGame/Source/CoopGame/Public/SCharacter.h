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
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	//UProperties 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	//These are Toggle Templates bound to action inputs.
	//Sprint
	void ToggleSprint(RunState speed);
	template<RunState speed>
	void ToggleSprint() {ToggleSprint(speed);}

	//Crouch function.
	void ToggleCrouch(CrouchState posture);
	template<CrouchState posture>
	void ToggleCrouch() {ToggleCrouch(posture);}

	//Jump
	void ToggleJump(JumpState jumping);	
	template<JumpState jumping>
	void ToggleJump() {ToggleJump(jumping);}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

};
