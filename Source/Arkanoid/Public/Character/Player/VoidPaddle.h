// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoidPaddleInterface.h"
#include "VoidPaddle.generated.h"


class AVoidLaser;
class UPaperFlipbook;
struct FInputActionValue;
class UFloatingPawnMovement;
class UBoxComponent;
class UPaperFlipbookComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;


DECLARE_MULTICAST_DELEGATE(FOnGameKickOffSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPaddleHitSideSignature, float, AActor*);

UCLASS()
class ARKANOID_API AVoidPaddle : public APawn, public IVoidPaddleInterface
{
	GENERATED_BODY()

public:

	AVoidPaddle();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComp) override;

	void ChangeToLaserPaddle_Implementation(float Duration) override;

	FOnGameKickOffSignature GameKickOffDelegate;

	FOnPaddleHitSideSignature PaddleHitSideDelegate;

	FVector PaddleVelocity;

	bool bGameStarted = false;

	void ChangeLevel();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;*/
	

private:

	void Move(const FInputActionValue& Value);

	void LaserPowerUp();
	
	void PressedSpace();

	UFUNCTION()
	void OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;

	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;*/

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> SpaceAction;

	UPROPERTY(EditAnywhere, Category = "Properties|Movement")
	float Speed = 500.f;
	
	FVector PaddleDirection;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<UPaperFlipbook> BasePaddle;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<UPaperFlipbook> LaserPaddle;
	
	bool bLaserPowerUpIsActive = false;
	
	bool bCanShoot = true;

	UPROPERTY(EditAnywhere, Category = "Properties|Laser")
	float LaserSpawnInterval = 0.3f;

	FTimerHandle LaserPowerUpTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Properties|Laser")
	TSubclassOf<AVoidLaser> LaserClass;

	

	
	
};




