// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VoidBrick.h"
#include "PaperSpriteComponent.h"
#include "Actor/VoidPowerUpBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Data/PowerUpData.h"
#include "Game/VoidGameInstanceInterface.h"


AVoidBrick::AVoidBrick()
{
	
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(BoxComponent);

	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	PaperSpriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComponent->SetupAttachment(GetRootComponent());


}

bool AVoidBrick::CanBeDestroyed()
{
	return bDestructible;
}

void AVoidBrick::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentHit.AddDynamic(this, &AVoidBrick::OnBoxHit);
	
}

void AVoidBrick::SpawnPowerUp()
{
	if (PowerUpData)
	{
				
		float RandNum = FMath::RandRange(0.f, 1.f);
					
		if (RandNum <= PowerUpData->DropChance)
		{
			

			float TotalWeight = 0.f;
					
			for (const FPowerUpInfo& Info  : PowerUpData->PowerUpsInfo)
			{
				
				TotalWeight += Info.Weight;
			}

			float RandWeight = FMath::RandRange(0.f, TotalWeight);

			float AccumulatedWeight = 0.f;
					
			for (const FPowerUpInfo& Info  : PowerUpData->PowerUpsInfo)
			{
				AccumulatedWeight += Info.Weight;
						
				if (RandWeight <= AccumulatedWeight)
				{
					FVector CalcLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 15.f, GetActorLocation().Z);
					
					GetWorld()->SpawnActor<AVoidPowerUpBase>(Info.PowerUpClass, CalcLocation, GetActorRotation());
					
					break;
				}
			}
		
		}
	
	}
}

void AVoidBrick::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor->ActorHasTag(FName("Laser")) || OtherActor->ActorHasTag(FName("Ball"))) && bDestructible)
	{
		CurrentNumberOfHitToDestruct++;
		if (CurrentNumberOfHitToDestruct >= NumberOfHitToDestruct)
		{
			OnDestroyedDelegate.Broadcast(this);
			
			if (IVoidGameInstanceInterface* GI_Interface = Cast<IVoidGameInstanceInterface>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				GI_Interface->UpdatePlayerScore(Score);
			}

			SpawnPowerUp();
			
			Destroy();
		}
		
	}
	
}


void AVoidBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

