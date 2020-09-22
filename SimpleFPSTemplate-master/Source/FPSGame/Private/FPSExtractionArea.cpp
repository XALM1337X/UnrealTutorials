// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionArea.h"
#include "Components/BoxComponent.h"

// Sets default values
AFPSExtractionArea::AFPSExtractionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	BoxOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxOverlap"));
	BoxOverlap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxOverlap->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
	RootComponent = BoxOverlap;
	BoxOverlap->SetHiddenInGame(false);
	BoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionArea::HandleOverlap);

}

// Called when the game starts or when spawned
void AFPSExtractionArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSExtractionArea::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverLap"));
}

// Called every frame
void AFPSExtractionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

