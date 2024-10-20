// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/Effects/PeriodicInstantEffect.h"

UPeriodicInstantEffect::UPeriodicInstantEffect(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	Period(1.0f)
{
}

void UPeriodicInstantEffect::RunPeriodicTimer()
{
	FTimerManager& manager = GetWorld()->GetTimerManager();
	if (manager.IsTimerActive(PeriodicTimerHandle))
		manager.ClearTimer(PeriodicTimerHandle);
	manager.SetTimer(PeriodicTimerHandle, FTimerDelegate::CreateUObject(this, &UPeriodicInstantEffect::PeriodTick),
	                 FMath::IsNearlyZero(Period) ? 0.1f : Period, true);
}

void UPeriodicInstantEffect::StartWork_Implementation()
{
	Super::StartWork_Implementation();
	RunPeriodicTimer();
}

FString UPeriodicInstantEffect::GetDebugString_Implementation() const
{
	return FString(TEXT("PeriodicInstantEffect"));
}

void UPeriodicInstantEffect::PeriodTick_Implementation()
{
}
