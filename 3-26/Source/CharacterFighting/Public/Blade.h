// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blade.generated.h"

UCLASS()
class CHARACTERFIGHTING_API ABlade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ÃÑµ¹ ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;

	// ¿Ü°ü ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UStaticMeshComponent* bodyMeshComp;
};
