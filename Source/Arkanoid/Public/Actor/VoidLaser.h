// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoidLaser.generated.h"

class UBoxComponent;
class UPaperSpriteComponent;

UCLASS()
class ARKANOID_API AVoidLaser : public AActor
{
	GENERATED_BODY()
	
public:
	
	AVoidLaser();

	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;
	

private:

	UFUNCTION()
	void OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperSpriteComponent> Laser1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPaperSpriteComponent> Laser2;

	UPROPERTY(EditAnywhere, Category = "Properteis")
	float Speed = 400.f;

	

};
