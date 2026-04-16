// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoidPowerUpBase.generated.h"

class UBoxComponent;
class UPaperFlipbookComponent;

UCLASS()
class ARKANOID_API AVoidPowerUpBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AVoidPowerUpBase();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherCompm, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent;

	UPROPERTY(EditAnywhere,Category = "Properties")
	float PowerUpSpeed = 500.f;

	FVector PowerUpVelocity;

	

};
