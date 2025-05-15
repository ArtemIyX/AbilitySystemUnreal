// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/Effects/PeriodicEffect.h"

UPeriodicEffect::UPeriodicEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), Period(1.0f)
{
}

void UPeriodicEffect::RunPeriodicTimer()
{
	FTimerManager& manager = GetWorld()->GetTimerManager();
	if (manager.IsTimerActive(PeriodicTimerHandle))
		manager.ClearTimer(PeriodicTimerHandle);
	manager.SetTimer(PeriodicTimerHandle, FTimerDelegate::CreateUObject(this, &UPeriodicEffect::PeriodTick),
	                 FMath::IsNearlyZero(Period) ? 0.1f : Period, true);
}

void UPeriodicEffect::ClearPeriodicTimer()
{
	FTimerManager& manager = GetWorld()->GetTimerManager();
	if (manager.IsTimerActive(PeriodicTimerHandle))
		manager.ClearTimer(PeriodicTimerHandle);
}

void UPeriodicEffect::StartWork_Implementation()
{
	Super::StartWork_Implementation();
	RunPeriodicTimer();
}

void UPeriodicEffect::OnWorkEnded_Implementation()
{
	Super::OnWorkEnded_Implementation();
	ClearPeriodicTimer();
}

void UPeriodicEffect::PeriodTick_Implementation()
{
	// Tick
}

FString UPeriodicEffect::GetDebugString_Implementation() const
{
	return FString::Printf(TEXT("PeriodicEffect"));
}
