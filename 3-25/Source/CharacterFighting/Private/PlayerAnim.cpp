// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "RPGPlayer.h"
#include <GameFramework/Controller.h>


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// �÷��̾� �� ��������
	auto pawn = TryGetPawnOwner();
	// ĳ����
	auto player = Cast<ARPGPlayer>(pawn);
	if (player)
	{
		// �ӵ� �� �������
		FVector velocity = player->GetVelocity();
		/** �̹����� �÷��̾� ���� ���� �ƴ϶� ��Ʈ�ѷ� ���� �̵�,ȸ���̹Ƿ� �÷��̾� ���� �ִ� Controller �� ������ ���ϴ� ������ ������ ��*/
		FVector forwardVector = pawn->GetActorForwardVector();
		// speed�� ���� �Ҵ��ϱ�
		// ������ ����(������)�� �ؼ� ���� �ٶ󺸰� �ִ� ������ ���Ѵ� 
		// �ӵ� ���� * ĳ������ ���� ���� = ���� �ٶ󺸰� �ִ� �� ����
		walk_speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	}
}
