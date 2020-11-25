// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDTAIController.h"

/**
 * 
 */
class SOFTDESIGNTRAINING_API SquadManager
{
public:
	SquadManager();
	~SquadManager();

	void AddPawn(ASDTAIController* controller);
	void RemovePawn(ASDTAIController* controller);
	bool IsMember(ASDTAIController* controller);
	void UpdateSightLoss();

	void ComputeTargetPositions();

private:
	TArray<ASDTAIController*> m_squadMembers;
	TArray<FVector> m_targetPositions;
};
