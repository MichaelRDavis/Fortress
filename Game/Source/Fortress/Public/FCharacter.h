// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "FCharacterBase.h"
#include "FCharacter.generated.h"

class UCameraComponent;
class UAbilitySystemComponent;
class UFCharacterMovement;
class AFInventoryItem;
class AFWeapon;
class AFUsable;

UCLASS(config=Game)
class FORTRESS_API AFCharacter : public AFCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFCharacter(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystem;
	}

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Pawn)
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character)
	UFCharacterMovement* FCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UAbilitySystemComponent* AbilitySystem;

protected:
	virtual void BeginPlay() override;

	AFUsable* GetUsableInView();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	float UseDistance;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pawn)
	bool bIsUsableInFocus;
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
	AFUsable* UsableInFocus;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Add item to inventory */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Pawn)
	virtual void AddItem(AFInventoryItem* Item);

	/** Remove item from inventory */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Pawn)
	virtual void RemoveItem(AFInventoryItem* Item);

	/** Find item in inventory */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual AFInventoryItem* FindItem(TSubclassOf<AFInventoryItem> ItemClass);

	/** Equips weapon from inventory */
	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void EquipWeapon(AFWeapon* Weap);

	/** Handles moving forward/backward */
	virtual void MoveForward(float Value);

	/** Handles strafing movement left/right */
	virtual void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void Sprint();
	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void StopSprinting();

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void Use();
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerUse();
	virtual void ServerUse_Implementation();
	virtual bool ServerUse_Validate();

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void StartFire();
	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void StopFire();

	UFUNCTION(BlueprintCallable, Category = Pawn)
	virtual void Reload();

	/** Check if pawn can fire primary weapon */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Pawn)
	virtual bool CanFire() const;

	/** Check if pawn can reload primary weapon */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Pawn)
	virtual bool CanReload();

protected:
	/** Items in inventory */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Pawn)
	TArray<AFInventoryItem*> Inventory;

	/** Currently equipped weapon */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Pawn)
	AFWeapon* Weapon;
	UPROPERTY(BlueprintReadOnly, Category = Pawn)
	AFWeapon* SecondaryWeapon;

	/** Sound played on jump */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	USoundBase* JumpSound;

	/** Sound played on sprint */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn)
	USoundBase* SprintSound;
};
