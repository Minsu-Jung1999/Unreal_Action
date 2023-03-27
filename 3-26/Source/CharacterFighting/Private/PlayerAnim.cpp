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
		// 속도 값 갖고오기
		FVector velocity = RpgPlayer->GetVelocity();
		/** 이번에는 플레이어 기준 앞이 아니라 컨트롤러 기준 이동,회전이므로 플레이어 폰에 있는 Controller 에 전면을 구하는 로직을 실험해 봄*/
		FVector forwardVector = Pawn->GetActorForwardVector();
		// speed에 내적 할당하기
		// 벡터의 내적(곱연산)을 해서 현재 바라보고 있는 방향을 구한다 
		// 속도 벡터 * 캐릭터의 방향 정보 = 현재 바라보고 있는 앞 방향
		walk_speed = FVector::DotProduct(forwardVector, velocity);

		FVector rightVector = RpgPlayer->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	}
}
