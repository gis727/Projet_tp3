// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayerIsPoweredUp.h"
#include "../SDTAIController.h"
#include "../SDTUtils.h"

EBTNodeResult::Type UBTTask_PlayerIsPoweredUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return SDTUtils::IsPlayerPoweredUp(GetWorld()) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}
