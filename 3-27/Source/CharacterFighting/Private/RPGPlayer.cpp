
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

	// Character Mesh ���� �Ҵ��� �ֱ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("SkeletalMesh'/Game/Animation/FBX/Mesh/Ch44_nonPBR.Ch44_nonPBR'"));
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

	// ī�޶� ���� �¿� �̵�
	PlayerInputComponent->BindAxis("Turn", this, &ARPGPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ARPGPlayer::LookUp);

	// ĳ���� ��,�� �̵�
	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGPlayer::MoveRight);

	// ���콺 Ŭ�� �̺�Ʈó��
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &ARPGPlayer::LMB_Down);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &ARPGPlayer::LMB_Up);

	// �޸��� ó��
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ARPGPlayer::InputRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ARPGPlayer::InputRun);

	// ���� �����ϱ�
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
		bAttacking = true;	// bLMB_DOWN �� ������ �����ϰ� �ִϸ��̼ǰ� ��Ÿ�ְ� ������ �Ǿ� �ִٸ� (������ ���� �׻� �̷������� Ȯ�� �� ����)

		AnimInstance->Montage_Play(attackMontage);
		const char* comboList[] = { "Attack_1","Attack_2","Attack_3"};		// ���ڿ� ������ �迭�� ��Ÿ�ֿ� �����ߴ� ���� ��� �̸��� ����
		/** �ִϸ��̼��� ���������� ���� ��*/
		AnimInstance->Montage_Play(attackMontage);	// �ִϸ��̼��� ���������� �ʴٸ� Montage_Play�� ����ؼ� ���� �տ� �� �� �����Ѵ�.
		if (AnimInstance->Montage_IsPlaying(attackMontage))
		{
			AnimInstance->Montage_JumpToSection(FName(comboList[comboCnt]), attackMontage); // ���� �迭�� �ִ� ����� �����Ų��.
		}
	}
}


void ARPGPlayer::ComboAttackCheck()
{

	
	if (bComboAttack)		// �޺� ������ ������ �ȴٸ� -> MainAnim_BP�� Event_Graph ���� ������ ���� ����Ѵ�.
	{
		comboCnt++;
		if (comboCnt > 2)	// �޺� ���� �� ���� �ʵ��� 
		{
			comboCnt = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("Combo Attack is available Combocount ++"));
		bComboAttack = false;
		Attack(); // �޺� ���� ���� 
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
