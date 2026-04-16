// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoidBall.generated.h"

class AVoidPaddle;
class UPaperSpriteComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallDisabledSignature, AVoidBall*, BallToDestroy);

UCLASS()
class ARKANOID_API AVoidBall : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AVoidBall();

	virtual void Tick(float DeltaTime) override;

	void EnableBall(bool bEnabled);

	FOnBallDisabledSignature OnBallDisabledDelegate;

	

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComp;

	
private:

	UFUNCTION()
	void OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperSpriteComponent> PaperSpriteComp;

	// This value add to paddle.Z - BallDestroyZOffset for Disable ball
	UPROPERTY(EditAnywhere, Category = "Properties")
	float BallDestroyZOffset = 20.f;
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	float BallSpeed = 1000.f;
	float TempBallSpeed;
	
	FVector BallDirection;
	
	FVector BallVelocity;

	bool bIsHit = false;

	bool bIsDisabled = false;


public:

	void SetBallVelocity(const FVector& NewVelocity);

	FVector GetBallVelocity() const;

	float GetBallSpeed() const;

	void SetBallSpeed(float InSpeed);
	

	

};


