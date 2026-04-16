// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VoidGameInstanceInterface.generated.h"

class UVoidSaveGame;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVoidGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARKANOID_API IVoidGameInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 GetPlayerLevel() const = 0;

	virtual void AddToLevel(int32 LevelToAdd)= 0;

	virtual void InitializePlayerVaus()= 0;

	virtual void InitializeScoreAndHighScore()= 0;

	virtual void UpdatePlayerScore(int32 InPlayerScore) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdatePlayerVaus(int32 VausToAdd);

	virtual int32 GetPlayerVaus() const = 0;

	virtual UVoidSaveGame* GetSaveGame() = 0;

	virtual bool HasSaveGame() = 0;
};
