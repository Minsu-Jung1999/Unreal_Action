// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPGPlayer.generated.h"

UCLASS()
class CHARACTERFIGHTING_API ARPGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 카메라 스피링 암
	UPROPERTY(VisibleAnywhere, Category = Camerar)
	class USpringArmComponent* springArm;
	
	// 카메라
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Camerar)
	class UCameraComponent* camcomp;

	// 카메라 회전 제어
	void Turn(float value);
	void LookUp(float value);
	// 카메라 회전 비율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camerar)
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camerar)
	float BaseLookUpRate;


	// 캐릭터 이동 제어
	void MoveForward(float value);
	void MoveRight(float value);

	// 마우스 클릭 이벤트
	void L_MouseLick();

	// 달리기 구현
	void InputRun();
	//void StartRun();
	//void StopRun();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category = Move)
	float run_speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Move)
	float walk_speed;

	
};
