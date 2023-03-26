
#include "RPGPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
// Sets default values
ARPGPlayer::ARPGPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 동적 할당해 주기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'"));
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
	PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &ARPGPlayer::L_MouseLick);

	// 달리기 처리
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ARPGPlayer::InputRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ARPGPlayer::InputRun);


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
	if ((Controller != NULL) && (value != 0.0f))
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
	if ((Controller != NULL) && (value != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, value);

	}
}

void ARPGPlayer::L_MouseLick()
{
	UE_LOG(LogTemp, Warning, TEXT("MouseClicked"));
}

void ARPGPlayer::InputRun()
{
	UE_LOG(LogTemp, Warning, TEXT("Running"));
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
