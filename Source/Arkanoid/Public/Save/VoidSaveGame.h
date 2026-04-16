// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VoidSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_API UVoidSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 HighScore;
	
};
