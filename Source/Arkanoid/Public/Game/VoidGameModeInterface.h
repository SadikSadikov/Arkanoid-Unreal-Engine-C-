// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VoidGameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVoidGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARKANOID_API IVoidGameModeInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ChangeAllBallSpeed(float NewSpeed, float Duration);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetDefaultSpeed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnBalls(int32 Number, float Duration);
	
};
