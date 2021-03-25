// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBotHealthComp.h"
#include "AI/STrackerBot.h"

USTrackerBotHealthComp::USTrackerBotHealthComp() {
	this->defaultHealth = 100;
	this->health = 0;
	SetIsReplicated(true);
}

void USTrackerBotHealthComp::BeginPlay() {
    Super::BeginPlay();
    if (GetOwnerRole() == ROLE_Authority) {
		AActor* myOwner = GetOwner();
		myOwner->OnTakePointDamage.AddDynamic(this, &USTrackerBotHealthComp::HandleTakePointDamageTrackerBot);	
		myOwner->OnTakeRadialDamage.AddDynamic(this, &USTrackerBotHealthComp::HandleTakeRadialDamageTrackerBot);
        this->health = this->defaultHealth;
	}
}

void USTrackerBotHealthComp::HandleTakePointDamageTrackerBot(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser ) {
    UE_LOG(LogTemp, Warning, TEXT("TrackerBotPointDamage"));
    this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);
    AActor* myOwner = GetOwner();
    if (myOwner) {
        ASTrackerBot* tb_act = Cast<ASTrackerBot>(myOwner);
        if (tb_act) {
            UStaticMeshComponent* tb_mesh = tb_act->mesh; //->CreateAndSetMaterialInstanceDynamicFromMaterial(0, mesh->GetMaterial(0));
            if (tb_act->MatInst == nullptr) {
                tb_act->MatInst = tb_mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, tb_mesh->GetMaterial(0));
            } else {
                tb_act->MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
            }
            
        }
    }


	if (this->health <= 0.0f) {
        //PlayExplosion effect && Destroy 
        GetOwner()->Destroy();
    }
}

void USTrackerBotHealthComp::HandleTakeRadialDamageTrackerBot(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser) {
    UE_LOG(LogTemp, Warning, TEXT("TrackerBotRadialDamage"));
        this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);

	if (this->health <= 0.0f) {
        //PlayExplosion effect && Destroy 
        GetOwner()->Destroy();
    }
}

