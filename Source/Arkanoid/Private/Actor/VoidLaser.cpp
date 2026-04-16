// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VoidLaser.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"



AVoidLaser::AVoidLaser()
{

	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	SetRootComponent(BoxComponent);

	Laser1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Laser1"));
	Laser1->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Laser1->SetupAttachment(GetRootComponent());

	Laser2 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Laser2"));
	Laser2->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Laser2->SetupAttachment(GetRootComponent());

}


void AVoidLaser::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Laser"));

	BoxComponent->OnComponentHit.AddDynamic(this, &AVoidLaser::OnBoxHit);
	
}

void AVoidLaser::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}


void AVoidLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Direction = FVector(0.f, 0.f, 1.f);

	const FVector Velocity = Speed * Direction;

	SetActorLocation(GetActorLocation() + Velocity * DeltaTime, true);

}

