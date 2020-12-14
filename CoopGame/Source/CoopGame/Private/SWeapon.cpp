// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void ASWeapon::Fire_Implementation()
{

	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		FVector EyeLocation; 
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector TraceEndPos = EyeLocation + (EyeRotation.Vector() * 10000);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(MyOwner);
		queryParams.AddIgnoredActor(this);
		queryParams.bTraceComplex = true;

		FHitResult res;
		if (GetWorld()->LineTraceSingleByChannel(res, EyeLocation, TraceEndPos, ECC_Visibility))
		{
			// Process damage and such
		}
		DrawDebugLine(GetWorld(), EyeLocation, TraceEndPos, FColor::White, false, 1.0f, 0, 1.0f);

	}
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

