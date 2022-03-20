// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_CharacterAnims.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_CharacterAnims : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECTC_API IInterface_CharacterAnims
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (AllowPrivateAccess = true))
	void Crouching(bool enabled);
};
