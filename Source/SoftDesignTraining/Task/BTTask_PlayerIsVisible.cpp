// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayerIsVisible.h"
#include "../SDTAIController.h"

EBTNodeResult::Type UBTTask_PlayerIsVisible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{
		return aiController->FoundPlayer() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
