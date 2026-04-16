// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PowerUpData.generated.h"

USTRUCT(BlueprintType)
struct FPowerUpInfo
{
	GENERATED_BODY()
	
	FPowerUpInfo(){}

	// The PowerUp class that will be spawned if this entry is selected
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AVoidPowerUpBase> PowerUpClass;

	// Relative weight used for weighted random selection.
	// Higher value = higher chance compared to other entries.
	// Range is flexible (e.g. 1, 2, 5 or 10, 50, 100, 200).
	// IMPORTANT: This is NOT a percentage.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 1.f;

	
	
};


UCLASS()
class ARKANOID_API UPowerUpData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPowerUpInfo> PowerUpsInfo;

	// Chance for this entry to be considered at all (0.0 - 1.0).
	// 0 = never spawn, 1 = always allowed (before weight selection).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DropChance = 0.1;


	// Curve used to scale PowerUp properties over time or progression (e.g. level, difficulty).
	// X-axis typically represents progression (level, time, or attempts),
	// Y-axis represents a multiplier applied to values such as Weight or DropChance.
	// Example: value = BaseValue * PowerUpMultiplier->GetFloatValue(CurrentLevel);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCurveFloat> PowerUpMultiplier = nullptr;

	
	
};
