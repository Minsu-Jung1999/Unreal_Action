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
	// 플레이어 이동속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float walk_speed = 0;

	// 매 프래임 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		class ARPGPlayer* RpgPlayer;

	
	
};
