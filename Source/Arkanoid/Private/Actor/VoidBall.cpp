// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VoidBall.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


AVoidBall::AVoidBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Ball"));

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(SphereComp);

	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComponent"));
	PaperSpriteComp->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	PaperSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComp->SetupAttachment(GetRootComponent());

}

void AVoidBall::EnableBall(bool bEnabled)
{
	SetActorTickEnabled(bEnabled);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, bEnabled]()
	{
		SetActorEnableCollision(bEnabled);
	}), 0.1f, false);
	
}


void AVoidBall::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Ball"));

	TempBallSpeed = BallSpeed;

	BallDirection = FVector(1.f, 0.f, 1.f).GetSafeNormal();
	
	BallVelocity = BallDirection * TempBallSpeed;
	
	SphereComp->OnComponentHit.AddDynamic(this, &AVoidBall::OnBoxHit);

	
}

void AVoidBall::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (!bIsHit)
	{
		bIsHit = true;
		
		BallVelocity = UKismetMathLibrary::MirrorVectorByNormal(BallVelocity, Hit.ImpactNormal);

		BallVelocity = BallVelocity.GetSafeNormal() * TempBallSpeed;

		// When paddle moves into the ball, multiple hits can occur each tick.
		// This logic ensures the collision is handled once and prevents the ball from getting stuck.
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			bIsHit = false;
			
		}), 0.1f, false);
	}
	

	
}

void AVoidBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BallVelocity = BallVelocity.GetSafeNormal() * TempBallSpeed;
	
	const FVector NewLoc = GetActorLocation() + (BallVelocity * DeltaTime);

	SetActorLocation(NewLoc, true);

	const float PawnZLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation().Z;
	
	if (NewLoc.Z < PawnZLocation - BallDestroyZOffset && !bIsDisabled)
	{
		bIsDisabled = true;
		OnBallDisabledDelegate.Broadcast(this);
		SetActorEnableCollision(false);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			Destroy();
			
		}), 2.f, false);
		
	}
	
	

}


void AVoidBall::SetBallVelocity(const FVector& NewVelocity)
{
	BallVelocity = NewVelocity;
}

FVector AVoidBall::GetBallVelocity() const
{
	return BallVelocity;
}

float AVoidBall::GetBallSpeed() const
{
	return BallSpeed;
}

void AVoidBall::SetBallSpeed(float InSpeed)
{
	TempBallSpeed = InSpeed;
}



