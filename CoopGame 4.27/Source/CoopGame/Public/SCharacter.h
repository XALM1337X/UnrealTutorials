// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAmmoChangeSigniture, int, AmmoCount, int, ClipCount, int, MaxClipSize, FString, WeaponName);


enum class WeaponSelectedState: uint8 {
	Primary,
	Secondary
};
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


UENUM(BlueprintType)
enum class AimState : uint8 
{
	Zoom,
	Hip
};
UENUM(BlueprintType)
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


USTRUCT()
struct FRunningStateReplicator {
	GENERATED_BODY()
	public: 
		UPROPERTY()
		bool sprintMsg;
};


UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

//Public Member Variables
public:


	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnAmmoChangeSigniture onAmmoChanged;
	
//Public Methods/Functions
public:	
	ASCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category="Player")
	bool GetIsAiming();

	UFUNCTION(BlueprintCallable, Category="Player")
	bool GetFiringState();

	UFUNCTION(Server, Reliable)
	void ServerSetFiringState(bool value);

	void SetFiringState(bool value);

	UFUNCTION(BlueprintCallable, Category="Player")
	ASWeapon* GetWeapon();

	USHealthComp* GetHealthComponent(); 

	

//Protected Member Variables
protected:
	//UProperties 
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComp* HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASWeapon> starterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASWeapon> launcherWeaponClass;

	FName WeaponAttachSocketName;

	UPROPERTY(Replicated, BlueprintReadWrite)
	ASWeapon* CurrentWeapon;

	ASWeapon* StoredPrimary;

	ASWeapon* StoredLauncher;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	bool isAiming;

	UPROPERTY(EditDefaultsOnly, Category="Player")
	float zoomPOV;

	UPROPERTY(EditDefaultsOnly, Category="Player", meta = ( ClampMin = 0.1, ClampMax = 100.0))
	float zoomInterpolationSpeed;

	bool isCrouching;
	
	float hipPOV;

	bool isFiring;
	
	UPROPERTY(BlueprintReadWrite, Category="Player")
	bool isSprinting;

	FTimerHandle th_time_between_shots;

	FTimerHandle th_recoil_tick_timer;

	UPROPERTY(ReplicatedUsing=OnRep_RunStateChange)
	FRunningStateReplicator run_replicator;

//Protected Methods/Functions
protected:
	// Called when the game starts or when spawned
	
	UFUNCTION()
	void OnRep_RunStateChange();

	virtual void BeginPlay() override;
	
	void ToggleCrouch();
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable)
	void TickUpRecoil();
	
	UFUNCTION(Server, Reliable)
	void TickDownRecoil();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void Reload();

	void Fire();
	//This function needs to be public so it can be called from blueprint
	UFUNCTION(BlueprintCallable, Category = "Player")
	void ToggleFire(FireState fire);
	template<FireState fire>
	void ToggleFire() {ToggleFire(fire);}
	

	UFUNCTION(Server, Reliable)
	void ServerSwitchToPrimary();
	void SwitchToPrimary();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToSecondary();
	void SwitchToSecondary();

	UFUNCTION(Server, Reliable)
	void ServerSetIsAiming(bool aim);
	void SetIsAiming(bool aim);

	//These are Toggle Templates bound to action inputs.
	//Sprint
	void ToggleSprint(RunState speed);
	template<RunState speed>
	void ToggleSprint() {ToggleSprint(speed);}

	UFUNCTION(Server, Reliable)
	void ServerToggleSprintOn();
	void SprintOn(); 

	UFUNCTION(Server, Reliable)
	void ServerToggleSprintOff();
	void SprintOff();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable) 
	void CastRunSpeed(int speed);

	//Jump
	void ToggleJump(JumpState jumping);	
	template<JumpState jumping>
	void ToggleJump() {ToggleJump(jumping);}

	UFUNCTION(BlueprintCallable, Category = "Player")
	void ToggleAim(AimState aim);
	template<AimState aim>
	void ToggleAim() {ToggleAim(aim);}


	void ToggleSwitchWeapon(WeaponSelectedState weapon);
	template<WeaponSelectedState weapon>
	void ToggleSwitchWeapon() {ToggleSwitchWeapon(weapon);}


};
