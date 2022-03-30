// Fill out your copyright notice in the Description page of Project Settings.


#include "Base.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABase::ABase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera_Boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	Camera_Boom->SetupAttachment(RootComponent);
	Camera_Boom->TargetArmLength = 200.f;
	Camera_Boom->bUsePawnControlRotation = true;
	Camera_Boom->bEnableCameraLag = true;
	Camera_Boom->CameraLagSpeed = 15.0f;
	Follow_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	Follow_Camera->AttachToComponent(Camera_Boom, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true) , USpringArmComponent::SocketName);
	Character_Movement = GetCharacterMovement();
	Character_Movement->bOrientRotationToMovement = true;
	Character_Mesh = GetMesh();
	Running = true;
	Sprinting = false;
	IsCrouching = false;

	Weapon_Sheath_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Sheath 1"));
	Weapon_Sheath_1->AttachToComponent(Character_Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true) , FName(TEXT("Weapon Sheath 1")));
	Weapon_In_Hand = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon In Hand"));
	Weapon_In_Hand->AttachToComponent(Character_Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true) , FName(TEXT("Weapon Hand Socket")));
	Dragon_Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dragon Sword"));
	Dragon_Sword->AttachToComponent(Weapon_Sheath_1, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
	Dragon_Sword->SetRelativeLocation(FVector::ZeroVector);
	Dragon_Sword->SetRelativeRotation(FRotator::ZeroRotator);
	NextUUID = 0;
	WeaponDrawn = false;
	MovementOn = true;
	//LightAttacked = false;
	//HeavyAttacked = false;
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABase::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABase::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Toggle Walk/Run", IE_Pressed, this, &ABase::ToggleWalkRun);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABase::SetSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABase::UnSetSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABase::ToggleCrouching);

	PlayerInputComponent->BindAction("Draw Weapon", IE_Pressed, this, &ABase::DrawWeapon);
	PlayerInputComponent->BindAction("Light Attack", IE_Pressed, this, &ABase::StartLightAttack1);
	PlayerInputComponent->BindAction("Heavy Attack", IE_Pressed, this, &ABase::StartHeavyAttack1);

}

bool ABase::InvertBoolean(bool value)
{
	if (value == true)
	{
		return false;
	}
	return true;
}

int32 ABase::GetNextUUID()
{
	return NextUUID++;
}

void ABase::MoveForward(float AxisValue)
{
	if (MovementOn == true)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void ABase::MoveRight(float AxisValue)
{
	if (MovementOn == true)
	{
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void ABase::ToggleWalkRun()
{
	Running = InvertBoolean(Running);
	if (Running == true)
	{
		CrouchOn(false);
		Character_Movement->MaxWalkSpeed = 600.0f;
	}
	else
	{
		Character_Movement->MaxWalkSpeed = 300.0f;
	}
}

void ABase::RunOn_Implementation(bool enabled)
{
	Running = InvertBoolean(enabled);
	ToggleWalkRun();
}

void ABase::SetSprint()
{
	CrouchOn(false);
	Sprinting = true;
	Character_Movement->MaxWalkSpeed = 900.0f;
}

void ABase::UnSetSprint()
{
	Sprinting = false;
	Character_Movement->MaxWalkSpeed = 600.0f;
}

void ABase::SprintOn_Implementation(bool enabled)
{
	if (enabled == true)
	{
		SetSprint();
	}
	else
	{
		UnSetSprint();
	}
}

void ABase::ToggleCrouching()
{
	IsCrouching = InvertBoolean(IsCrouching);
	if (IsCrouching == true)
	{
		RunOn(false);
		Crouch();
	}
	else
	{
		UnCrouch();
	}
	UAnimInstance* AnimInstance = Character_Mesh->GetAnimInstance();
	if (AnimInstance->GetClass()->ImplementsInterface(UInterface_CharacterAnims::StaticClass())) {
		IInterface_CharacterAnims::Execute_Crouching(AnimInstance,IsCrouching);
	}
}

void ABase::CrouchOn_Implementation(bool enabled)
{
	IsCrouching = InvertBoolean(enabled);
	ToggleCrouching();
}

void ABase::DrawWeapon()
{
	if (Equip_Weapon_Montage)
	{
		WeaponDrawn = InvertBoolean(WeaponDrawn);
		PlayAnimMontage(Equip_Weapon_Montage, 2.0f);
		FLatentActionInfo info;
		FTimerDelegate TimerDelegate;
		if (WeaponDrawn == true)
		{
			TimerDelegate.BindLambda([&]
			{
				Dragon_Sword->AttachToComponent(Weapon_In_Hand, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), FName(TEXT("Weapon Hand Socket")));
				info.CallbackTarget = this;
				info.Linkage = 0;
				info.UUID = GetNextUUID();
				/*
				if (LightAttacked == true)
				{
					info.ExecutionFunction = FName("PlayLightAttack1Montage");
					LightAttacked = false;
				}
				*/
				UKismetSystemLibrary::MoveComponentTo(Dragon_Sword, FVector::ZeroVector, FRotator::ZeroRotator, false, false, 0.3f, false, EMoveComponentAction::Move,info);
			});
		}
		else
		{
			TimerDelegate.BindLambda([&]
			{
				Dragon_Sword->AttachToComponent(Weapon_Sheath_1, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), FName(TEXT("Weapon Sheath 1")));
				info.CallbackTarget = this;
				info.Linkage = 0;
				info.UUID = GetNextUUID();
				UKismetSystemLibrary::MoveComponentTo(Dragon_Sword, FVector::ZeroVector, FRotator::ZeroRotator, false, false, 0.3f, false, EMoveComponentAction::Move, info);
			});
		}
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.4f, false);
	}
}

void ABase::WeaponOn_Implementation(bool drawn)
{
	if (Equip_Weapon_Montage)
	{
		WeaponDrawn = InvertBoolean(drawn);
		DrawWeapon();
	}

}

/*
void ABase::PlayLightAttack1Montage()
{
	if (Light_Attack_1_Montage)
	{
		PlayAnimMontage(Light_Attack_1_Montage, 1.0f);
	}
}
*/

void ABase::StartLightAttack1()
{
	//LightAttacked = true;
	if (WeaponDrawn == true)
	{
		PlayAnimMontage(Light_Attack_1_Montage, 1.6f);
		//LightAttacked = false;
	}
	else
	{
		WeaponOn(true);
	}
}

void ABase::StartHeavyAttack1()
{
	if (WeaponDrawn == true)
	{
		PlayAnimMontage(Heavy_Attack_1_Montage, 1.0f);
	}
	else
	{
		WeaponOn(true);
	}
}

void ABase::Movement_Implementation(bool on)
{
	MovementOn = on;
}
