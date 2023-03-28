
#include "RPGPlayer.h"
#include "Blade.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Animation/AnimInstance.h>
#include <Components/SkeletalMeshComponent.h>
#include <Engine/SkeletalMeshSocket.h>
// Sets default values
ARPGPlayer::ARPGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Character Mesh 동적 할당해 주기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("SkeletalMesh'/Game/Animation/FBX/Mesh/Ch44_nonPBR.Ch44_nonPBR'"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -87.0f), FRotator(0, -90, 0));
	}
	
	// 스프링암과 카메라 동적 할당해 주기
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	camcomp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));


	// 스프링암을 루트 시에 붙여주기
	springArm->SetupAttachment(RootComponent);
	// 카메라는 스프링 암에 붙여주기
	camcomp->SetupAttachment(springArm);

	// 스프링암 위치 설정해 주기 (BP_PlayerSetting 참고)
	springArm->SetRelativeLocation(FVector(0, 0, 90));
	springArm->TargetArmLength = 400;

	// RPG 특성에 맞게 카메라와 캐릭터에 회전을 분리해 주기 위해서 하는 설정 
	bUseControllerRotationYaw = false;
	springArm->bUsePawnControlRotation = true;
	camcomp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 기본 걷기 속도 저장
	walk_speed = 600;
	GetCharacterMovement()->MaxWalkSpeed = walk_speed;

	// 달리기 속도 저장
	run_speed = 700;

	// 회전 값 초기화
	BaseTurnRate = 100;
	BaseLookUpRate = 100;

	bLMBDown = false;
	bAttacking = false;	
	comboCnt = 0;
}

// Called when the game starts or when spawned
void ARPGPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARPGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 카메라 상하 좌우 이동
	PlayerInputComponent->BindAxis("Turn", this, &ARPGPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ARPGPlayer::LookUp);

	// 캐릭터 앞,뒤 이동
	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGPlayer::MoveRight);

	// 마우스 클릭 이벤트처리
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &ARPGPlayer::LMB_Down);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &ARPGPlayer::LMB_Up);

	// 달리기 처리
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ARPGPlayer::InputRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ARPGPlayer::InputRun);

	// 무기 장착하기
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ARPGPlayer::Equip);


}

void ARPGPlayer::Turn(float value)
{
	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayer::LookUp(float value)
{
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGPlayer::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && (!bAttacking))
	{
		// 컨트롤러에 현재 회전 값을 구한다. ( 캐릭터 중심 이동이 아닌 카메라 중심 이동구현을 위해서)
		FRotator Rotation = Controller->GetControlRotation();
		
		// 카메라를 바닥, 땅을 볼 때 캐릭터가 움직이지 않는 버그를 수정하기 위해 x, z 값에 직접 0 을 넣어준다.
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction,value);

	}

}

void ARPGPlayer::MoveRight(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && (!bAttacking))
	{
		FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, value);

	}
}

void ARPGPlayer::LMB_Down()
{
	bLMBDown = true;
	if (!bAttacking)
	{
		Attack();
	}
	else
	{
		bComboAttack = true;
	}
}
void ARPGPlayer::LMB_Up()
{
	bLMBDown = false;
}



void ARPGPlayer::Attack()
{
	bLMBDown = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && attackMontage)
	{
		bAttacking = true;	// bLMB_DOWN 을 참으로 변경하고 애니메이션과 몽타주가 연결이 되어 있다면 (포인터 사용시 항상 이런식으로 확인 후 진행)

		AnimInstance->Montage_Play(attackMontage);
		const char* comboList[] = { "Attack_1","Attack_2","Attack_3"};		// 문자열 포인터 배열로 몽타주에 저장했던 공격 모션 이름들 저장
		/** 애니메이션이 실행중이지 않을 때*/
		AnimInstance->Montage_Play(attackMontage);	// 애니메이션이 실행중이지 않다면 Montage_Play를 사용해서 가장 앞에 거 만 진행한다.
		if (AnimInstance->Montage_IsPlaying(attackMontage))
		{
			AnimInstance->Montage_JumpToSection(FName(comboList[comboCnt]), attackMontage); // 다음 배열에 있는 모션을 실행시킨다.
		}
	}
}


void ARPGPlayer::ComboAttackCheck()
{

	
	if (bComboAttack)		// 콤보 공격이 실행이 된다면 -> MainAnim_BP에 Event_Graph 에서 조정된 값을 사용한다.
	{
		comboCnt++;
		if (comboCnt > 2)	// 콤보 공격 값 넘지 않도록 
		{
			comboCnt = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("Combo Attack is available Combocount ++"));
		bComboAttack = false;
		Attack(); // 콤보 공격 실행 
	}
	
}


void ARPGPlayer::Equip()
{
	UE_LOG(LogTemp, Warning, TEXT("Equip"));	
	//FTransform RightHandSocket = GetMesh()->GetSocketTransform("RightHandSocket");
	//GetWorld()->SpawnActor<ABlade>(bladefactory, RightHandSocket);
	AActor* blade = GetWorld()->SpawnActor<ABlade>(bladefactory, GetMesh()->GetSocketTransform("RightHandSocket"));
	const USkeletalMeshSocket* _RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");
	_RightHandSocket->AttachActor(blade, GetMesh());
	
}

void ARPGPlayer::EndAttacking()
{
	bAttacking = false;
	comboCnt = 0;
}

void ARPGPlayer::InputRun()
{
	auto movement = GetCharacterMovement();

	// 달리는 중이라면
	if (movement->MaxWalkSpeed > walk_speed)
	{
		movement->MaxWalkSpeed = walk_speed;
	}
	else
	{
		movement->MaxWalkSpeed = run_speed;
	}
}

//void ARPGPlayer::StartRun()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Running"));
//	auto movement = GetCharacterMovement();
//	movement->MaxWalkSpeed = run_speed;
//	
//}
//
//void ARPGPlayer::StopRun()
//{
//	auto movement = GetCharacterMovement();
//	movement->MaxWalkSpeed = walk_speed;
//
//}
