// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetFleeLocation.h"
#include "../SDTAIController.h"

EBTNodeResult::Type UBTTask_TargetFleeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{
		const ASDTAIController::PlayerInteractionBehavior previousBehavior = aiController->m_PlayerInteractionBehavior;
		const ASDTAIController::PlayerInteractionBehavior newBehavior = ASDTAIController::PlayerInteractionBehavior_Flee;

		if (previousBehavior != newBehavior)
		{
			aiController->m_PlayerInteractionBehavior = newBehavior;
			aiController->AIStateInterrupted();
		}
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}