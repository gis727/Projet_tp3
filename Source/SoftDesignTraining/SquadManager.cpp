// Fill out your copyright notice in the Description page of Project Settings.


#include "SquadManager.h"
#include "SoftDesignTraining.h"

SquadManager::SquadManager()
{
	UE_LOG(LogTemp, Warning, TEXT("SquadManager created!")); // debug
}

SquadManager::~SquadManager()
{
	UE_LOG(LogTemp, Warning, TEXT("SquadManager destroyed!")); // debug
}

void SquadManager::AddPawn(ASDTAIController* controller)
{
	m_squadMembers.AddUnique(controller);
	ComputeTargetPositions();
}

void SquadManager::RemovePawn(ASDTAIController* controller)
{
	m_squadMembers.Remove(controller);
	controller->ResetSquadState();
}

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
			if (!member->AtJumpSegment)
			{
				member->m_PlayerInteractionBehavior = ASDTAIController::PlayerInteractionBehavior_Collect;
				member->AIStateInterrupted();
			}
		}
		m_squadMembers.Empty();
	}
}

bool SquadManager::IsMember(ASDTAIController* controller)
{
	return m_squadMembers.Contains(controller);
}

void SquadManager::ComputeTargetPositions()
{
	if (m_squadMembers.Num() < 2) return;

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(m_squadMembers[0]->GetWorld(), 0);
	if (!player) return;

	m_targetPositions.Empty();

	const int targetPoints = 4;
	const float angle = (2.f * PI) / (float) targetPoints;
	const float maxDistToPlayer = 200.f;
	float currentAngle = 0.f;

	for (int i = 0; i < targetPoints; i++)
	{
		FVector nextTarget = player->GetActorLocation();
		nextTarget.X += maxDistToPlayer * cos(currentAngle);
		nextTarget.Y += maxDistToPlayer * sin(currentAngle);

		m_targetPositions.Add(nextTarget);

		currentAngle += angle;
	}

	int currentTarget = 0;
	for (auto member : m_squadMembers)
	{
		// check if the pawn should be updated (not too close)
		const FVector toPlayer = player->GetActorLocation() - member->GetPawn()->GetActorLocation();
		const float distToPlayer = toPlayer.Size2D();

		if (distToPlayer > maxDistToPlayer && !member->AtJumpSegment)
		{
			// update the pawn
			member->UpdateSquadState(m_targetPositions[currentTarget]);
		}

		// set the next target
		currentTarget++;
		currentTarget = currentTarget < targetPoints ? currentTarget : 0;
	}
}
