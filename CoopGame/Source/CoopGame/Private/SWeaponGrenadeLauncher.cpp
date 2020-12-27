// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponGrenadeLauncher.h"
#include "GrenadeProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "kismet/GameplayStatics.h"

ASWeaponGrenadeLauncher::ASWeaponGrenadeLauncher()
{
    PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshCompGren"));
	MuzzleSocketName = "MuzzleSocket";
}

void ASWeaponGrenadeLauncher::Fire_Implementation() 
{
	AActor* MyOwner = GetOwner();
	APawn*  pawn = Cast<APawn>(MyOwner);
	if (MyOwner)
	{

		if (MuzzleEffect) 
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
			
		}	

		if (ProjectileClass)
		{
			FVector EyeLocation; 
			FRotator EyeRotation;		
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);


			FVector ShotDirection = EyeRotation.Vector();

			FVector TraceEndPos = EyeLocation + (ShotDirection * 10000);

			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotator = EyeRotation;
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;


			FHitResult hit;
			FVector TraceEndPoint = TraceEndPos;
			if (GetWorld()->LineTraceSingleByChannel(hit, EyeLocation, TraceEndPos, ECC_Visibility))
			{
				//AActor* HitActor = hit.GetActor();
				TraceEndPoint = hit.ImpactPoint;

			}

			if (pawn)
			{
				ActorSpawnParams.Instigator = pawn;
				// spawn the projectile at the muzzle
				FRotator FinalRot  =  (TraceEndPoint - MuzzleLocation).Rotation();
				GetWorld()->SpawnActor<AGrenadeProjectile>(ProjectileClass, MuzzleLocation, FinalRot, ActorSpawnParams);
			}
		}

	}
}



// Called when the game starts or when spawned
void ASWeaponGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeaponGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

