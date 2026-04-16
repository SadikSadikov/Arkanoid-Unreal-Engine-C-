// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoidEnemy.generated.h"

UCLASS()
class ARKANOID_API AVoidEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AVoidEnemy();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

};
