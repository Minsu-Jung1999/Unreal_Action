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

	// ī�޶� ���Ǹ� ��
	UPROPERTY(VisibleAnywhere, Category = Camerar)
	class USpringArmComponent* springArm;
	
	// ī�޶�
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Camerar)
	class UCameraComponent* camcomp;

	// ī�޶� ȸ�� ����
	void Turn(float value);
	void LookUp(float value);
	// ī�޶� ȸ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camerar)
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camerar)
	float BaseLookUpRate;


	// ĳ���� �̵� ����
	void MoveForward(float value);
	void MoveRight(float value);

	// ���콺 Ŭ�� �̺�Ʈ
	void LMB_Down();
	void LMB_Up();
	bool bLMBDown;

	// �޸��� ����
	void InputRun();
	//void StartRun();
	//void StopRun();
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category = Move)
	float run_speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Move)
	float walk_speed;

	// �����Լ�
	void Attack();

	UFUNCTION(BlueprintCallable)
	void EndAttacking();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;
	int comboCnt;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = Attack)
	class UAnimMontage* attackMontage;

	// �޺����� �Լ�
	UFUNCTION(BlueprintCallable)
	void ComboAttackCheck();
	bool bComboAttack;


	// ���� ����
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		TSubclassOf<class ABlade> weaponFactory;
	
	// ���� �����ϱ�
	void Equip();
};
