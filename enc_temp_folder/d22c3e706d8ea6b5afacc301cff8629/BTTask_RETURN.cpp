// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RETURN.h"


UBTTask_RETURN::UBTTask_RETURN()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}


EBTNodeResult::Type UBTTask_RETURN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RETURN);

	//UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	//if (nullptr != MoveCom)
	//{
	//	MoveCom->MaxWalkSpeed = 500.0f;
	//}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	FVector originPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
	FVector returnDir = originPos - GetGlobalCharacter(OwnerComp)->GetActorLocation();
	GetGlobalCharacter(OwnerComp)->AddActorLocalOffset(returnDir * 500.0f * DelataSeconds);
}
