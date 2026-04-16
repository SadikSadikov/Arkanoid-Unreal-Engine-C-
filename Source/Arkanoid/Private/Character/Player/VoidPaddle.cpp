// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/VoidPaddle.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Actor/VoidLaser.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


AVoidPaddle::AVoidPaddle()
{

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_Pawn);
	SetRootComponent(BoxComponent);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
	FlipbookComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	FlipbookComponent->SetupAttachment(GetRootComponent());
	
	/*SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);*/
}

void AVoidPaddle::SetupPlayerInputComponent(UInputComponent* InputComp)
{
	Super::SetupPlayerInputComponent(InputComp);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComp))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVoidPaddle::Move);

		EnhancedInputComponent->BindAction(SpaceAction, ETriggerEvent::Started, this, &AVoidPaddle::PressedSpace);
	}
}

void AVoidPaddle::ChangeToLaserPaddle_Implementation(float Duration)
{
	if (!bLaserPowerUpIsActive)
	{
		FlipbookComponent->SetFlipbook(LaserPaddle);

		bLaserPowerUpIsActive = true;

		
		GetWorld()->GetTimerManager().SetTimer(LaserPowerUpTimerHandle,this,  &AVoidPaddle::LaserPowerUp, Duration, false);
	}
	

	
}

void AVoidPaddle::ChangeLevel()
{
	GetWorld()->GetTimerManager().ClearTimer(LaserPowerUpTimerHandle);

	bLaserPowerUpIsActive = false;
		
	FlipbookComponent->SetFlipbook(BasePaddle);
}

void AVoidPaddle::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputLocalPlayerSubsystem->AddMappingContext(InputMapping, 0);
		}
	}

	BoxComponent->OnComponentHit.AddDynamic(this, &AVoidPaddle::OnBoxHit);


	
}

void AVoidPaddle::Move(const FInputActionValue& Value)
{
	const float ScaleValue = Value.Get<float>();

	PaddleDirection = FVector(ScaleValue, 0, 0).GetSafeNormal();
	PaddleVelocity = PaddleDirection * Speed;

	const FVector NewLocation = GetActorLocation() + PaddleVelocity * UGameplayStatics::GetWorldDeltaSeconds(this);
	//SetActorLocation(NewLocation, true);

	AddActorWorldOffset(PaddleVelocity * UGameplayStatics::GetWorldDeltaSeconds(this), true);
	
}

void AVoidPaddle::LaserPowerUp()
{

	bLaserPowerUpIsActive = false;
		
	FlipbookComponent->SetFlipbook(BasePaddle);
}

void AVoidPaddle::PressedSpace()
{
	if (!bGameStarted)
	{
		GameKickOffDelegate.Broadcast();

		bGameStarted = true;
		return;
	}

	if (bLaserPowerUpIsActive && bCanShoot)
	{
		bCanShoot = false;
		
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			bCanShoot = true;
			
		}), LaserSpawnInterval, false);

		if (LaserClass)
		{
			GetWorld()->SpawnActor<AVoidLaser>(LaserClass, GetActorLocation(), GetActorRotation());
		}
		
	}
	
}

void AVoidPaddle::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag(FName("Ball"))) return;
	
	const FVector HitLocalLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Hit.ImpactPoint);

	// if alpha is 1 this means ball is hit on the right, -1 on the left and 0 is middle
	const float Alpha = HitLocalLocation.X / BoxComponent->GetScaledBoxExtent().X;

	PaddleHitSideDelegate.Broadcast(Alpha, OtherActor);
	
	
	
}
