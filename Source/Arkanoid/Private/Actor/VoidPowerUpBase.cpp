// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VoidPowerUpBase.h"

#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

AVoidPowerUpBase::AVoidPowerUpBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SetRootComponent(BoxComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	FlipbookComponent->SetupAttachment(GetRootComponent());
	
}


void AVoidPowerUpBase::BeginPlay()
{
	Super::BeginPlay();

	PowerUpVelocity = FVector(0.f, 0.f, -1.f) * PowerUpSpeed;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AVoidPowerUpBase::OnBoxBeginOverlap);
	
}

void AVoidPowerUpBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


void AVoidPowerUpBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector NewLocation = GetActorLocation() + PowerUpVelocity * DeltaTime;
	
	SetActorLocation(NewLocation);

}

