// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/Effects/DurationEffect.h"

UDurationEffect::UDurationEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Time = 10.0f;
}

void UDurationEffect::RunTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		EndWork();
	}), FMath::IsNearlyZero(Time) ? 0.1f : Time, false);
}

FString UDurationEffect::GetDebugString_Implementation() const
{
	return FString::Printf(TEXT("DurationEffect"));
}

void UDurationEffect::StartWork_Implementation()
{
	RunTimer();
}

bool UDurationEffect::Stack_Implementation(UEffect* AnotherEffect)
{
	if (IsValid(AnotherEffect))
	{
		RunTimer();
		return true;
	}
	return false;
}

void UDurationEffect::OnWorkEnded_Implementation()
{
	FTimerManager& manager = GetWorld()->GetTimerManager();
	if (manager.IsTimerActive(TimerHandle))
		manager.ClearTimer(TimerHandle);
}
