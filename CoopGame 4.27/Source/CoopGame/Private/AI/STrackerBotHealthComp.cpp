// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBotHealthComp.h"
#include "AI/STrackerBot.h"
#include "SGameMode.h"
#include "SCharacter.h"

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
    AActor* HC = Cast<AActor>(this);
    if (HC && HC->IsActorBeingDestroyed()) {
        return;
    }
    this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);

    AActor* myOwner = GetOwner();
    if (myOwner) {
        ASTrackerBot* tb_act = Cast<ASTrackerBot>(myOwner);
        if (tb_act) {
            tb_act->PlayImpulseEffect();
            if (this->health <= 0.0f) {             
                
                ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());                    
                if (GM) {
                    if (InstigatedBy) {
                        GM->ScoreBroadcast(DamagedActor, DamageCauser, InstigatedBy);
                    }
                }
                
                //PlayExplosion effect && Destroy 
            tb_act->ServerExplode();
            }
        }
    }
    
}

void USTrackerBotHealthComp::HandleTakeRadialDamageTrackerBot(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser) {
    AActor* HC = Cast<AActor>(this);
    if (HC && HC->IsActorBeingDestroyed()) {
        return;
    }
    this->health = FMath::Clamp(this->health - Damage, 0.0f, this->defaultHealth);
    AActor* myOwner = GetOwner();
    if (myOwner) {
        ASTrackerBot* tb_act = Cast<ASTrackerBot>(myOwner);
        if (this->health <= 0.0f && tb_act) {                
            tb_act->ServerExplode();              
        }
    }    
}

