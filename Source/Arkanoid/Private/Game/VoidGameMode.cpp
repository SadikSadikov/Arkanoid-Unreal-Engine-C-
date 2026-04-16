// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VoidGameMode.h"
#include "Actor/VoidBall.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/VoidPaddle.h"
#include "Game/VoidGameInstanceInterface.h"
#include "Kismet/GameplayStatics.h"


void AVoidGameMode::ChangeAllBallSpeed_Implementation(float NewSpeed, float Duration)
{

	if (!bChangedBallSped)
	{
		bChangedBallSped = true;

		for (AVoidBall* Ball : AvaliableBalls)
		{
			Ball->SetBallSpeed(NewSpeed);
		}
		

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			bChangedBallSped = false;

			for (AVoidBall* Ball : AvaliableBalls)
			{
				Ball->SetBallSpeed(AvaliableBalls[0]->GetBallSpeed());
			}
			
		}), Duration, false);
	}
	
	
}

float AVoidGameMode::GetDefaultSpeed_Implementation()
{
	if (AvaliableBalls.Num() > 0)
	{
		return AvaliableBalls[0]->GetBallSpeed();
	}

	return 1000.f;
	
}

void AVoidGameMode::SpawnBalls_Implementation(int32 Number, float Duration)
{
	if (BallClass && !bSpawnedBalls && AvaliableBalls.Num() > 0 && Number >= 0)
	{
		bSpawnedBalls = true;
		
		for (int32 i = 0; i < Number; i++)
		{
			FVector BallLoc = AvaliableBalls[0]->GetActorLocation();
		
			FVector FixedBallLoc = FVector(BallLoc.X, BallLoc.Y, BallLoc.Z + SpawnBallZOffset);

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
			AVoidBall* SpawnedBall = GetWorld()->SpawnActor<AVoidBall>(BallClass, FixedBallLoc, AvaliableBalls[0]->GetActorRotation(), SpawnParams);

			SpawnedBall->OnBallDisabledDelegate.AddDynamic(this, &AVoidGameMode::OnBallDisabled);
		
			SpawnedBall->EnableBall(true);
	
			AvaliableBalls.Add(SpawnedBall);
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			bSpawnedBalls = false;

			for (int32 i = AvaliableBalls.Num() - 1; i > 0; i--)
			{
				AvaliableBalls[i]->Destroy();
				AvaliableBalls.RemoveAt(i);
			}
			
			
		}), Duration, false);

	}
}

void AVoidGameMode::SpawnBall()
{
	if (BallClass)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		
		FVector PawnLoc = PlayerPawn->GetActorLocation();
		FVector FixedPawnLoc = FVector(PawnLoc.X, PawnLoc.Y - 10.f, PawnLoc.Z + SpawnBallZOffset);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
		AVoidBall* SpawnedBall = GetWorld()->SpawnActor<AVoidBall>(BallClass, FixedPawnLoc, PlayerPawn->GetActorRotation(), SpawnParams);

		SpawnedBall->OnBallDisabledDelegate.AddDynamic(this, &AVoidGameMode::OnBallDisabled);
		
		SpawnedBall->EnableBall(false);
	
		AvaliableBalls.Add(SpawnedBall);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld,
		                                          EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
		SpawnedBall->AttachToActor(PlayerPawn, AttachmentRules);
	}
}

void AVoidGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (BrickManagerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		BrickManager = GetWorld()->SpawnActor<AVoidBrickManager>(BrickManagerClass, BrickManagerLocation, FRotator::ZeroRotator, SpawnParams);

		BrickManager->OnChangeLevelSignature.AddUObject(this, &AVoidGameMode::ChangeLevel);


	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	VoidPaddle = Cast<AVoidPaddle>(PlayerPawn);

	VoidPaddle->GameKickOffDelegate.AddUObject(this, &AVoidGameMode::GameIsKickedOff);

	VoidPaddle->PaddleHitSideDelegate.AddUObject(this, &AVoidGameMode::PaddleIsHit);
	
	SpawnBall();

	

	if (OverlayClass)
	{
		OverlayWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), OverlayClass);
		
		OverlayWidget->AddToViewport();

	}

	if (IVoidGameInstanceInterface* GI_Interface = Cast<IVoidGameInstanceInterface>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI_Interface->InitializeScoreAndHighScore();
		
		GI_Interface->InitializePlayerVaus();
	}
	
	
}

void AVoidGameMode::ChangeLevel()
{
	if (IVoidGameInstanceInterface* GI_Interface = Cast<IVoidGameInstanceInterface>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (GI_Interface->GetPlayerLevel() == 3)
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName("L_GameOver"));
			return;
		}

		VoidPaddle->ChangeLevel();

		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		
		GI_Interface->AddToLevel(1);

		const FName* LevelName = Levels.Find(GI_Interface->GetPlayerLevel());

		if (Levels.Num() > 0 && LevelName)
		{
			UGameplayStatics::OpenLevel(GetWorld(), *LevelName);
		}

		
		
	}
}

void AVoidGameMode::RespawnBall()
{
	VoidPaddle->bGameStarted = false;
	SpawnBall();
}

void AVoidGameMode::GameIsKickedOff()
{
	if (AvaliableBalls.Num() > 0)
	{

		FDetachmentTransformRules DetachmentRiles(EDetachmentRule::KeepWorld,
			EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		AvaliableBalls[0]->DetachFromActor(DetachmentRiles);
		
		AvaliableBalls[0]->EnableBall(true);
	}
}

void AVoidGameMode::PaddleIsHit(float PaddleHitSideAlpha, AActor* HitBall)
{
	FVector NewDirection;

	if (PaddleHitSideAlpha <= 0.1f && PaddleHitSideAlpha >= -0.1f)
	{
		NewDirection = FVector(FMath::RandRange(0.1f, 0.2f), 0.f, 1.f);
		
	}
	else if (PaddleHitSideAlpha < -0.3)
	{
		NewDirection = FVector(-1.f, 0.f, 1.f);
		
	}
	else
	{
		NewDirection = FVector(1.f, 0.f, 1.f);
		
	}
	
	
	NewDirection = NewDirection.GetSafeNormal();

	for (AVoidBall* Ball : AvaliableBalls)
	{
		if (Ball == HitBall)
		{
			FVector NewVelocity = NewDirection * Ball->GetBallSpeed();
			Ball->SetBallVelocity(NewVelocity);
			
		}
		
	}
	
}

void AVoidGameMode::OnBallDisabled(AVoidBall* BallToDestroy)
{

	AvaliableBalls.Remove(BallToDestroy);
	
	if (AvaliableBalls.Num() <= 0)
	{
		if (IVoidGameInstanceInterface* GI_Interface = Cast<IVoidGameInstanceInterface>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			GI_Interface->Execute_UpdatePlayerVaus(UGameplayStatics::GetGameInstance(GetWorld()), -1);
			
			if (GI_Interface->GetPlayerVaus() > 0)
			{
				
				GetWorld()->GetTimerManager().SetTimer(RespawnBallTimerHandle, this, &AVoidGameMode::RespawnBall, 1.f, false);
			}
			
		}
		
	}
}
