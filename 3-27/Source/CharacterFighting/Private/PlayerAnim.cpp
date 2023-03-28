// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "RPGPlayer.h"
#include <GameFramework/Controller.h>


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			RpgPlayer = Cast<ARPGPlayer>(Pawn);
		}
	}
	if (RpgPlayer)
	{
		// �ӵ� �� �������
		FVector velocity = RpgPlayer->GetVelocity();
		/** �̹����� �÷��̾� ���� ���� �ƴ϶� ��Ʈ�ѷ� ���� �̵�,ȸ���̹Ƿ� �÷��̾� ���� �ִ� Controller �� ������ ���ϴ� ������ ������ ��*/
		FVector forwardVector = Pawn->GetActorForwardVector();
		// speed�� ���� �Ҵ��ϱ�
		// ������ ����(������)�� �ؼ� ���� �ٶ󺸰� �ִ� ������ ���Ѵ� 
		// �ӵ� ���� * ĳ������ ���� ���� = ���� �ٶ󺸰� �ִ� �� ����
		walk_speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = RpgPlayer->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	}
}
