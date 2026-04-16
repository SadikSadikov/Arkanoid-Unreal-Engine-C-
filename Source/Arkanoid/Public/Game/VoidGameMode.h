// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoidGameModeInterface.h"
#include "Actor/Manager/VoidBrickManager.h"
#include "GameFramework/GameModeBase.h"
#include "VoidGameMode.generated.h"

class AVoidPaddle;
class AVoidBall;
class UUserWidget;
/**
 * 
 */
UCLASS()
class ARKANOID_API AVoidGameMode : public AGameModeBase, public IVoidGameModeInterface
{
	GENERATED_BODY()

public:

	virtual void ChangeAllBallSpeed_Implementation(float NewSpeed, float Duration) override;

	virtual float GetDefaultSpeed_Implementation() override;

	virtual void SpawnBalls_Implementation(int32 Number, float Duration) override;
	void SpawnBall();

protected:

	virtual void BeginPlay() override;

private:

	void GameIsKickedOff();

	void PaddleIsHit(float PaddleHitSideAlpha, AActor* HitBall);

	UFUNCTION()
	void OnBallDisabled(AVoidBall* BallToDestroy);

	void ChangeLevel();

	void RespawnBall();

	UPROPERTY(EditAnywhere, Category = "Properties|Level")
	TMap<int32, FName> Levels;
	
	UPROPERTY(EditAnywhere, Category = "Properties|Ball")
	TSubclassOf<AVoidBall> BallClass;

	UPROPERTY()
	TArray<AVoidBall*> AvaliableBalls;

	UPROPERTY(EditAnywhere, Category = "Properties|Ball")
	float SpawnBallZOffset = 38.f;

	UPROPERTY(EditAnywhere, Category = "Properties|Brick")
	TSubclassOf<AVoidBrickManager> BrickManagerClass;

	UPROPERTY(EditAnywhere, Category = "Properties|Brick")
	FVector BrickManagerLocation = FVector::ZeroVector;

	UPROPERTY()
	AVoidBrickManager* BrickManager;

	bool bChangedBallSped = false;

	bool bSpawnedBalls = false;

	UPROPERTY(EditDefaultsOnly, Category = "Properties|UI")
	TSubclassOf<UUserWidget> OverlayClass;

	UPROPERTY()
	UUserWidget* OverlayWidget;

	UPROPERTY()
	AVoidPaddle* VoidPaddle;

	FTimerHandle RespawnBallTimerHandle;
	
	
};


