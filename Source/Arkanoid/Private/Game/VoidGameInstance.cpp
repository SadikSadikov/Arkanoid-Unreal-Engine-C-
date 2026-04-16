// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VoidGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/VoidSaveGame.h"

void UVoidGameInstance::Init()
{
	Super::Init();

	LoadGame();

	if (VoidSaveGame)
	{
		HighScore = VoidSaveGame->HighScore;
	}
	
	
}

int32 UVoidGameInstance::GetPlayerLevel() const
{
	return Level;
}

void UVoidGameInstance::AddToLevel(int32 LevelToAdd)
{
	Level += LevelToAdd;
}

void UVoidGameInstance::InitializePlayerVaus()
{
	OnVausChangedDelegate.Broadcast(Vaus);
}

void UVoidGameInstance::InitializeScoreAndHighScore()
{
	OnScoreChangedDelegate.Broadcast(Score);

	OnHighScoreChangedDelegate.Broadcast(HighScore);
}

void UVoidGameInstance::UpdatePlayerScore(int32 InPlayerScore)
{
	Score += InPlayerScore;
	OnScoreChangedDelegate.Broadcast(Score);

	if (VoidSaveGame && Score >= HighScore)
	{
		HighScore = Score;
		
		OnHighScoreChangedDelegate.Broadcast(HighScore);
		
		VoidSaveGame->HighScore = HighScore;
		
		SaveGame();
	}
}

void UVoidGameInstance::UpdatePlayerVaus_Implementation(int32 VausToAdd)
{
	Vaus += VausToAdd;

	OnVausChangedDelegate.Broadcast(Vaus);

	if (Vaus <= 0)
	{
		OnGameOverDelegate.Broadcast();
	}
}

int32 UVoidGameInstance::GetPlayerVaus() const
{
	return Vaus;
}

UVoidSaveGame* UVoidGameInstance::GetSaveGame()
{
	return VoidSaveGame;
}

bool UVoidGameInstance::HasSaveGame()
{
	return bHasSave;
}

void UVoidGameInstance::RestartGame()
{
	Vaus = 2;
	Score = 0;
	Level = 1;
}

void UVoidGameInstance::LoadGame()
{
	bHasSave = UGameplayStatics::DoesSaveGameExist(FString("SAVE"), 0);

	if (bHasSave)
	{
		VoidSaveGame = Cast<UVoidSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("SAVE"), 0));
	}
	else
	{
		VoidSaveGame = Cast<UVoidSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
}

void UVoidGameInstance::SaveGame()
{
	if (VoidSaveGame)
	{
		
		bHasSave = UGameplayStatics::SaveGameToSlot(VoidSaveGame, FString("SAVE"), 0);
		
	}
	
}
