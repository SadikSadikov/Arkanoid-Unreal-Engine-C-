// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoidGameInstanceInterface.h"
#include "Engine/GameInstance.h"
#include "VoidGameInstance.generated.h"

class UVoidSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChangedSignature, int32, Score);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighScoreChangedSignature, int32, HighScore);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVausChangedSignature, int32, Vaus);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverSignature);

UCLASS()
class ARKANOID_API UVoidGameInstance : public UGameInstance, public IVoidGameInstanceInterface
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	virtual int32 GetPlayerLevel() const  override;

	virtual void AddToLevel(int32 LevelToAdd) override;

	virtual void InitializePlayerVaus() override;

	virtual void InitializeScoreAndHighScore() override;

	virtual void UpdatePlayerScore(int32 InPlayerScore) override;

	void UpdatePlayerVaus_Implementation(int32 VausToAdd);

	virtual int32 GetPlayerVaus() const override;

	virtual UVoidSaveGame* GetSaveGame() override;

	virtual bool HasSaveGame() override;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChangedSignature OnScoreChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHighScoreChangedSignature OnHighScoreChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnVausChangedSignature OnVausChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnGameOverSignature OnGameOverDelegate;

	UFUNCTION(BlueprintCallable)
	void RestartGame();



protected:

private:

	void LoadGame();

	void SaveGame();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UVoidSaveGame> SaveGameClass;
	
	UPROPERTY()
	TObjectPtr<UVoidSaveGame> VoidSaveGame;

	bool bHasSave = false;

	int32 Vaus = 2;

	int32 Level = 1;

	int32 Score;

	int32 HighScore;
	
};



