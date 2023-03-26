
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

	// Mesh ���� �Ҵ��� �ֱ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -87.0f), FRotator(0, -90, 0));
	}
	
	// �������ϰ� ī�޶� ���� �Ҵ��� �ֱ�
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	camcomp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// ���������� ��Ʈ �ÿ� �ٿ��ֱ�
	springArm->SetupAttachment(RootComponent);
	// ī�޶�� ������ �Ͽ� �ٿ��ֱ�
	camcomp->SetupAttachment(springArm);

	// �������� ��ġ ������ �ֱ� (BP_PlayerSetting ����)
	springArm->SetRelativeLocation(FVector(0, 0, 90));
	springArm->TargetArmLength = 400;

	// RPG Ư���� �°� ī�޶�� ĳ���Ϳ� ȸ���� �и��� �ֱ� ���ؼ� �ϴ� ���� 
	bUseControllerRotationYaw = false;
	springArm->bUsePawnControlRotation = true;
	camcomp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// �⺻ �ȱ� �ӵ� ����
	walk_speed = 600;
	GetCharacterMovement()->MaxWalkSpeed = walk_speed;

	// �޸��� �ӵ� ����
	run_speed = 700;

	// ȸ�� �� �ʱ�ȭ
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

	// ī�޶� ���� �¿� �̵�
	PlayerInputComponent->BindAxis("Turn", this, &ARPGPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ARPGPlayer::LookUp);

	// ĳ���� ��,�� �̵�
	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGPlayer::MoveRight);

	// ���콺 Ŭ�� �̺�Ʈó��
	PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &ARPGPlayer::L_MouseLick);

	// �޸��� ó��
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
		// ��Ʈ�ѷ��� ���� ȸ�� ���� ���Ѵ�. ( ĳ���� �߽� �̵��� �ƴ� ī�޶� �߽� �̵������� ���ؼ�)
		FRotator Rotation = Controller->GetControlRotation();
		
		// ī�޶� �ٴ�, ���� �� �� ĳ���Ͱ� �������� �ʴ� ���׸� �����ϱ� ���� x, z ���� ���� 0 �� �־��ش�.
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
	
	// �޸��� ���̶��
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
