// Fill out your copyright notice in the Description page of Project Settings.

#include "FPickupItem.h"
#include "Components/StaticMeshComponent.h"

AFPickupItem::AFPickupItem()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	SetReplicates(true);
	bAlwaysRelevant = true;
	NetUpdateFrequency = 1.0f;
	PrimaryActorTick.bCanEverTick = true;
}