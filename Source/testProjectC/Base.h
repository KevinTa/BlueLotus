// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface_CharacterAnims.h"
#include "Interface_CharacterActions.h"
#include "Base.generated.h"

UCLASS()
class TESTPROJECTC_API ABase : public ACharacter, public IInterface_CharacterActions
{
	GENERATED_BODY()
	//Movement Animation Properties
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* Camera_Boom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Follow_Camera;

	UPROPERTY(VisibleAnywhere)
	class UCharacterMovementComponent* Character_Movement;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Character_Mesh;

	UPROPERTY()
	bool Running;

	UPROPERTY()
	bool Sprinting;

	UPROPERTY()
	bool IsCrouching;

	//Combat Animation Properties
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Weapon_Sheath_1;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Weapon_In_Hand;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Dragon_Sword;

	UPROPERTY()
	int32 NextUUID;

	UPROPERTY()
	bool WeaponDrawn;

	UPROPERTY()
	bool MovementOn;

	UPROPERTY()
	int AttackCounter;

	UPROPERTY()
	bool ComboMode;

public:
	// Sets default values for this character's properties
	ABase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Helper Function to get a unique UUID for Latent Actions
	UFUNCTION()
	int32 GetNextUUID();

	//Movement Animation Functions
	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void ToggleWalkRun();

	UFUNCTION()
	void SetSprint();

	UFUNCTION()
	void UnSetSprint();

	UFUNCTION()
	void ToggleCrouching();

	//Movement Animation Interface Override Functions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void RunOn(bool enabled); virtual void RunOn_Implementation(bool enabled) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void SprintOn(bool enabled); virtual void SprintOn_Implementation(bool enabled) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void CrouchOn(bool enabled); virtual void CrouchOn_Implementation(bool enabled) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Equip_Weapon_Montage;

	//Combat Animation Functions
	UFUNCTION()
	void DrawWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void WeaponOn(bool drawn); virtual void WeaponOn_Implementation(bool drawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Light_Attack_1_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Light_Attack_2_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Light_Attack_3_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Heavy_Attack_1_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Heavy_Attack_2_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	class UAnimMontage* Heavy_Attack_3_Montage;

	UFUNCTION()
	void SetLightAttack1Delay(int select);

	UFUNCTION()
	void StartLightAttack1();

	UFUNCTION()
	void SetHeavyAttack1Delay(int select);

	UFUNCTION()
	void StartHeavyAttack1();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void Movement(bool on); virtual void Movement_Implementation(bool on) override;
};
