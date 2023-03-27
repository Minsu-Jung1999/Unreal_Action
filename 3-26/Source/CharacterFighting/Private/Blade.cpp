// Fill out your copyright notice in the Description page of Project Settings.


#include "Blade.h"
#include <Components/SphereComponent.h>

// Sets default values
ABlade::ABlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 1. �浹ü ����ϱ�
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	// 2. ��Ʈ�� ���
	RootComponent = collisionComp;

	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));;
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABlade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

