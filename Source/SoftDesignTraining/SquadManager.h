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
	float GetBestTargetIndex(ASDTAIController* member);
	void ComputeTargetPositions(ACharacter* player);
	void Distribute();
	void Reset();

private:

	const float m_maxDistToPlayer = 350.0f; // Rayon du cercle autour du joueur
	const int m_targetPoints = 4; // Nombre de positions sur le cercle

	TArray<ASDTAIController*> m_squadMembers;
	TArray<FVector> m_targetPositions;
	TArray<bool> m_targetPositionsState; // Indique si une position donnée est déja prise par un membre
	TArray<float> m_targetPositionsAngles; // Angles des positions autour du joueur
};
