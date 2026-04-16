// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/VoidGameModeInterface.h"
#include "GameFramework/Actor.h"
#include "VoidTypes/BirckTypes.h"
#include "VoidBrickManager.generated.h"

class ULevelPatternData;
class AVoidBrick;

DECLARE_MULTICAST_DELEGATE(FOnChangeLevelDelegate);

UCLASS()
class ARKANOID_API AVoidBrickManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AVoidBrickManager();

	virtual void Tick(float DeltaTime) override;

	FOnChangeLevelDelegate OnChangeLevelSignature;

protected:

	virtual void BeginPlay() override;

	// Map.Key is equals to level, Map.Value is the Data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	TMap<int32, ULevelPatternData*> LevelPatternData;

private:

	UFUNCTION()
	void DestroyedBrick(AVoidBrick* BrickToDestroy);

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SpacingX = 20;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SpacingZ = 20;

	UPROPERTY()
	TArray<AVoidBrick*> Bricks;


public:
	

	

	

	

};


