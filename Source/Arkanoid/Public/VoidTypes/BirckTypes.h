#pragma once

#include "CoreMinimal.h"
#include "BirckTypes.generated.h"

UENUM(BlueprintType)
enum class EBrickType : uint8
{
	EBT_White UMETA(DisplayName = "White"),
	EBT_Orange UMETA(DisplayName = "Orange"),
	EBT_Cyan UMETA(DisplayName = "Cyan"),
	EBT_Green UMETA(DisplayName = "Green"),
	EBT_Red UMETA(DisplayName = "Red"),
	EBT_Blue UMETA(DisplayName = "Blue"),
	EBT_Violet UMETA(DisplayName = "Violet"),
	EBT_Yellow UMETA(DisplayName = "Yellow"),
	EBT_Silver UMETA(DisplayName = "Silver"),
	EBT_Gold UMETA(DisplayName = "Gold"),
};

USTRUCT(BlueprintType)
struct FBrickSpawnData
{
	GENERATED_BODY()

public:
	
	FBrickSpawnData(){}
	
	// If value is true will be spawned if false not
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldSpawn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bShouldSpawn))
	TSubclassOf<class AVoidBrick> BrickClass;
	
};

USTRUCT(BlueprintType)
struct FBrickColumns
{
	GENERATED_BODY()

public:
	
	FBrickColumns(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBrickSpawnData> ColumnData;

};