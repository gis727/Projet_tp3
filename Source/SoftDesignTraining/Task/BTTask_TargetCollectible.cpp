// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetCollectible.h"
#include "../SDTAIController.h"

EBTNodeResult::Type UBTTask_TargetCollectible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{
		const ASDTAIController::PlayerInteractionBehavior previousBehavior = aiController->m_PlayerInteractionBehavior;
		const ASDTAIController::PlayerInteractionBehavior newBehavior = ASDTAIController::PlayerInteractionBehavior_Collect;

		if (previousBehavior != newBehavior)
		{

			aiController->m_PlayerInteractionBehavior = newBehavior;
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
