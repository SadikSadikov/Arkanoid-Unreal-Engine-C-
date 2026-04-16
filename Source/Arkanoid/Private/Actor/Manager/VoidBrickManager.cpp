// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Manager/VoidBrickManager.h"

#include "Actor/VoidBrick.h"
#include "Data/LevelPatternData.h"
#include "Game/VoidGameInstanceInterface.h"
#include "Kismet/GameplayStatics.h"


AVoidBrickManager::AVoidBrickManager()
{
 
	PrimaryActorTick.bCanEverTick = true;

}


void AVoidBrickManager::BeginPlay()
{
	Super::BeginPlay();

	const FVector StartLocation = GetActorLocation();

	const FRotator StartRotation = GetActorRotation();

	int32 PlayerLevel = 1;
	
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	
	if (IVoidGameInstanceInterface* GI_Interface = Cast<IVoidGameInstanceInterface>(GameInstance))
	{
		PlayerLevel = GI_Interface->GetPlayerLevel();
	}

	for (auto Pair : LevelPatternData)
	{
		if (Pair.Key == PlayerLevel /* this is current level*/)
		{
			for (int32 i = 0; i < Pair.Value->RowData.Num(); i++)
			{
				for (int32 j = 0; j < Pair.Value->RowData[i].ColumnData.Num(); j++)
				{
					if (Pair.Value->RowData[i].ColumnData[j].bShouldSpawn)
					{
						const float NewBrickX = StartLocation.X + (j * SpacingX);
						const float NewBrickZ = StartLocation.Z + (i * -SpacingZ);

						FVector NewBrickLocation = FVector(NewBrickX, 0.f , NewBrickZ);

						AVoidBrick* SpawnedBrick = GetWorld()->SpawnActor<AVoidBrick>(Pair.Value->RowData[i].ColumnData[j].BrickClass, NewBrickLocation, StartRotation);

						if (SpawnedBrick->CanBeDestroyed())
						{
							Bricks.Add(SpawnedBrick);
						}
						
					}
					
				}
			}
			
		}
	}

	for (AVoidBrick* Brick : Bricks)
	{
		Brick->OnDestroyedDelegate.AddDynamic(this, &AVoidBrickManager::DestroyedBrick);
	}
	
}

void AVoidBrickManager::DestroyedBrick(AVoidBrick* BrickToDestroy)
{
	if (Bricks.Contains(BrickToDestroy))
	{
		Bricks.Remove(BrickToDestroy);
		if (Bricks.Num() <= 0 )
		{
			OnChangeLevelSignature.Broadcast();
		}

		
	}
}


void AVoidBrickManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


