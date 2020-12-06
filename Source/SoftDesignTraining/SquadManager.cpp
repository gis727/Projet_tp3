// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadManager.h"
#include "SoftDesignTraining.h"

SquadManager::SquadManager()
{
	// Fill the positions angles
	const float angle = (2.f * PI) / (float)m_targetPoints;
	float currentAngle = 0.f;

	for (int i = 0; i < m_targetPoints; i++)
	{
		m_targetPositionsAngles.Add(currentAngle);
		currentAngle += angle;
	}
}

SquadManager::~SquadManager()
{
}

/*
 * Ajoute un membre
 */
void SquadManager::AddPawn(ASDTAIController* controller)
{
	m_squadMembers.AddUnique(controller);
	Distribute();
}

/*
 * Retire un membre
 */
void SquadManager::RemovePawn(ASDTAIController* controller)
{
	m_squadMembers.Remove(controller);
	controller->ResetSquadState();
}

/*
 * Met à jour le vision de l'escouade
 * Si plus aucun membre ne voit le joueur, l'escouade est dissoute
 */
void SquadManager::UpdateSightLoss()
{
	// Check if all players lost sight
	bool allMembersLostSight = true;
	for (auto member : m_squadMembers)
	{
		if (member->m_HasLosOnPlayer) allMembersLostSight = false;
	}

	if (allMembersLostSight)
	{
		// dismantle squad
		for (auto member : m_squadMembers)
		{
			member->m_PlayerInteractionBehavior = ASDTAIController::PlayerInteractionBehavior_Collect;
			if (!member->AtJumpSegment) member->AIStateInterrupted();
		}
		m_squadMembers.Empty();
	}
}

/*
 * Indique si le pawn d'un controller est un membre
 */
bool SquadManager::IsMember(ASDTAIController* controller)
{
	return m_squadMembers.Contains(controller);
}

/*
 * Pour un membre donné, trouve la première position libre la plus proche
 */
float SquadManager::GetBestTargetIndex(ASDTAIController* member)
{
	// Check if all squad positions are taken
	bool allPositionsAreTaken = true;
	for (bool targetPosition : m_targetPositionsState)
	{
		allPositionsAreTaken = allPositionsAreTaken && targetPosition;
	}

	// Get the closest available position
	float closestPositionIndex = -1;
	float distToClosestPosition = MAX_FLT;

	for (int posIndex = 0; posIndex < m_targetPositions.Num(); posIndex++)
	{
		const FVector targetPosition = m_targetPositions[posIndex];
		const bool positionTaken = m_targetPositionsState[posIndex];

		const float distToPosition = (targetPosition - member->GetPawn()->GetActorLocation()).Size2D();
		const bool positionCanBeConsidered = allPositionsAreTaken || !positionTaken;

		if (distToPosition < distToClosestPosition && positionCanBeConsidered)
		{
			distToClosestPosition = distToPosition;
			closestPositionIndex = posIndex;
		}
	}

	return closestPositionIndex;
}

/*
 * Efface les positions précédemment trouvées
 */
void SquadManager::Reset()
{
	m_targetPositions.Empty();
	m_targetPositionsState.Empty();

	for (float angle : m_targetPositionsAngles)
	{
		m_targetPositionsState.Add(false);
	}
}

/*
 * Trouve les positions à viser en fonction de la position du joueur
 */
void SquadManager::ComputeTargetPositions(ACharacter* player)
{
	// Compute the postions around the player
	for (float angle : m_targetPositionsAngles)
	{
		FVector nextTarget = player->GetActorLocation();
		nextTarget.X += m_maxDistToPlayer * cos(angle);
		nextTarget.Y += m_maxDistToPlayer * sin(angle);

		m_targetPositions.Add(nextTarget);
	}
}

/*
 * Distribue les positions aux membres de l'escouade
 */
void SquadManager::Distribute()
{
	if (m_squadMembers.Num() < 2) return;

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(m_squadMembers[0]->GetWorld(), 0);
	if (!player) return;

	Reset();
	ComputeTargetPositions(player);

	for (auto member : m_squadMembers)
	{
		// check if the member should be updated (not too close)
		const float distToPlayer = (player->GetActorLocation() - member->GetPawn()->GetActorLocation()).Size2D();

		if (distToPlayer > m_maxDistToPlayer && !member->AtJumpSegment)
		{
			// update the member
			const float bestPositionIndex = GetBestTargetIndex(member);
			if (bestPositionIndex >= 0)
			{
				member->UpdateSquadState(m_targetPositions[bestPositionIndex]);
				m_targetPositionsState[bestPositionIndex] = true; // let the other members know that this position is taken
			}
		}
	}
}
