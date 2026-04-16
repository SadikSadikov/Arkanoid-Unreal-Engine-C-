// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VoidTypes/BirckTypes.h"
#include "LevelPatternData.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_API ULevelPatternData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBrickColumns> RowData;
	
};
