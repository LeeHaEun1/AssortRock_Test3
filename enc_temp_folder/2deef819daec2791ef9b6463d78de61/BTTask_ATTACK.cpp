// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ATTACK.h"

UBTTask_ATTACK::UBTTask_ATTACK()
{
	// �� �����Ӹ��� �����ض�
	bNotifyTick = true;
	// ������ ���� �ѹ� �����ض�.
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ATTACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->SetAniState(UBTTask_AIBase::GetAiState(OwnerComp));


	return EBTNodeResult::Type::InProgress;
}

void UBTTask_ATTACK::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::MOVE);
	}
}