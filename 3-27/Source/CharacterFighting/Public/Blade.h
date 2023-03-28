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

	/** Base Shape Collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
		class USphereComponent* CollisionVolume;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
		class USkeletalMeshComponent* SkeletalMesh;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
