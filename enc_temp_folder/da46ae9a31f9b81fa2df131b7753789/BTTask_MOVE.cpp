// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MOVE.h"
#include <Global/GlobalCharacter.h>
#include <AI/AICon.h>
#include <Global/GlobalEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "GameFramework/CharacterMovementComponent.h"

// �ϴ� ��ġ ���� �ٱ����� �̵��ϸ�
// Return ���·� ��ȯ�ϴ� ���� 2���� ��Ȳ�� �����Ѵ�.
// 1. �׳� �÷��̾ �ʹ� �־����� ���� ���ư���.
// 2. �� �����ڸ�(������� ������ �ڽ��� ��ġ.)���� �ʹ� ���� ��������.

UBTTask_MOVE::UBTTask_MOVE()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MOVE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::MOVE);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 500.0f;
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_MOVE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (nullptr == TargetActor)
	{
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	{
		FVector TargetPos = TargetActor->GetActorLocation();
		FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		// Ȥ�ö� z���� ���� ���ɼ��� ���ִ°� �����Դϴ�.
		TargetPos.Z = 0.0f;
		ThisPos.Z = 0.0f;

		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}



	{
		FVector PawnPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector TargetPos = TargetActor->GetActorLocation();

		FVector Dir = TargetPos - PawnPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);
		//GetGlobalCharacter(OwnerComp)->SetActorRotation(Dir.Rotation());

		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		if (SearchRange < Dir.Size())
		{
			// ���� �󸮷� ���ư���
			SetStateChange(OwnerComp, AIState::IDLE);
			return;
		}

		// ����� �����ߴ�.
		if (AttackRange >= Dir.Size())
		{
			SetStateChange(OwnerComp, AIState::ATTACK);
			return;
		}
	}

	{
		FVector originPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		FVector offset = GetGlobalCharacter(OwnerComp)->GetActorLocation() - originPos;

		if (offset.Size() > SearchRange * 1.5f)
		{
			SetStateChange(OwnerComp, AIState::RETURN);
		}
	}

}