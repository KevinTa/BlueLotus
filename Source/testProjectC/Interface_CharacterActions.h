// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_CharacterActions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_CharacterActions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECTC_API IInterface_CharacterActions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void RunOn(bool enabled);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void SprintOn(bool enabled);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void CrouchOn(bool enabled);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void WeaponOn(bool drawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void Movement(bool on);
};
