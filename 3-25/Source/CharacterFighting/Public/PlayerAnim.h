// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERFIGHTING_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	// �÷��̾� �̵��ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float walk_speed = 0;

	// �� ������ ���ŵǴ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// �÷��̾� �¿� �̵� �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;

};
