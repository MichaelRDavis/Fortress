// Fill out your copyright notice in the Description page of Project Settings.

#include "FCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AFCharacterBase::AFCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);

	DeathCleanupTime = 10.0f;
	Health = 0;
	MaxHealth = 50;
	Shield = 0;
	MaxShield = 0;
	PrimaryActorTick.bCanEverTick = true;
}

void AFCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFCharacterBase, Health, COND_None);
	DOREPLIFETIME_CONDITION(AFCharacterBase, Shield, COND_None);
	DOREPLIFETIME_CONDITION(AFCharacterBase, LastTakeHitInfo, COND_None);
}

void AFCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (Health == 0)
	{
		Health = MaxHealth;
	}
}

void AFCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AFCharacterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.0f)
	{
		return 0.0f;
	}

	int32 ResultDamage = FMath::TruncToInt(Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser));
	if (ResultDamage > 0)
	{
		ModifyDamageTaken(ResultDamage);
		Health -= ResultDamage;
		if (Health <= 0)
		{
			Die(EventInstigator, DamageEvent, DamageCauser);
		}
		else
		{
			NotifyTakeHit(EventInstigator, Damage, DamageEvent);
		}
	}

	return (float)ResultDamage;
}

bool AFCharacterBase::K2_Die(AController* EventInstigator, TSubclassOf<UDamageType> DamageType)
{
	return Die(EventInstigator, FPointDamageEvent(Health + 1, FHitResult(), FVector(0.0f, 0.0f, -1.0f), DamageType));
}

bool AFCharacterBase::Die(AController* EvnetInstigator, const FDamageEvent& DamageEvent, AActor* DamageCauser /*= nullptr*/)
{
	if (Role < ROLE_Authority || IsDead())
	{
		return false;
	}
	else
	{
		Health = FMath::Min<int32>(0.0f, Health);
		bReplicateMovement = false;
		TearOff();
		bIsDead = true;

		Death();
		return true;
	}
}

void AFCharacterBase::SetTakeHitInfo(int32 Damage)
{
	const float TimeoutTIme = GetWorld()->GetTimeSeconds() + 0.5f;

	LastTakeHitInfo.Damage = Damage;
}

void AFCharacterBase::ModifyDamageTaken(int32& Damage)
{
	if (Damage > 0 && Shield > 0)
	{
		int32 AbsorbDamage = FMath::Min(Shield, FMath::Max<int32>(1, Damage * 0.5f));
		Damage -= AbsorbDamage;
		Shield = FMath::Max(0, Shield - AbsorbDamage);
	}
}

void AFCharacterBase::Death()
{
	TimeOfDeath = GetWorld()->TimeSeconds;

	if (GetNetMode() != NM_DedicatedServer && !IsPendingKillPending())
	{
		// Disabled ragdoll for performance 
		//StartRagdoll();

		if (!IsPendingKillPending())
		{

		}
	}
	else
	{
		SetLifeSpan(0.25f);
	}
}

void AFCharacterBase::PlayTakeHitEffects()
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		if (HitSound)
		{
			UGameplayStatics::SpawnSoundAttached(HitSound, GetRootComponent(), NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
		}
	}
}

void AFCharacterBase::NotifyTakeHit(AController* InstigatedBy, int32 Damage, const FDamageEvent& DamageEvent)
{
	if (Role == ROLE_Authority)
	{
		SetTakeHitInfo(Damage);
	}
}

void AFCharacterBase::StartRagdoll()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesNotifyRigidBodyCollision(true);
	GetMesh()->UpdateKinematicBonesToAnim(GetMesh()->GetComponentSpaceTransforms(), ETeleportType::TeleportPhysics, true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->RefreshBoneTransforms();
	GetMesh()->SetAllBodiesPhysicsBlendWeight(1.0f);
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	RootComponent = GetMesh();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetCapsuleComponent()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
}

void AFCharacterBase::StopRagdoll()
{

}

int32 AFCharacterBase::GetHealth() const
{
	return Health;
}

int32 AFCharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

bool AFCharacterBase::IsDead() const
{
	return Health < 0 || bIsDead;
}

void AFCharacterBase::OnRep_LastTakeHitInfo()
{
	
}

