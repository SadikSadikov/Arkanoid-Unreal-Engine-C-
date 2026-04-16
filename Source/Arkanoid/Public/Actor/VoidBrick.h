// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoidTypes/BirckTypes.h"
#include "VoidBrick.generated.h"

class AVoidBrickManager;
class UPowerUpData;
class UPaperSpriteComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyedSiganture, AVoidBrick*, BrickToDestroy);


UCLASS()
class ARKANOID_API AVoidBrick : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AVoidBrick();

	virtual void Tick(float DeltaTime) override;
	
	FOnDestroyedSiganture OnDestroyedDelegate;

	bool CanBeDestroyed();

protected:
	
	virtual void BeginPlay() override;
	void SpawnPowerUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperSpriteComponent> PaperSpriteComponent;

private:

	UFUNCTION()
	void OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Brick Properties")
	EBrickType BrickType = EBrickType::EBT_Silver;

	UPROPERTY(EditAnywhere, Category = "Brick Properties")
	bool bDestructible = true;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bDestructible"), Category = "Brick Properties")
	int32 Score = 50;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bDestructible"), Category = "Brick Properties")
	int32 NumberOfHitToDestruct = 1;

	int32 CurrentNumberOfHitToDestruct = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Brick Properties")
	TObjectPtr<UPowerUpData> PowerUpData;
	
	
	

};

