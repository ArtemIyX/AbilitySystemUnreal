﻿// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/Attribute.h"

#include "Components/ASComponent.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

UAttribute::UAttribute(const FObjectInitializer& InObjectInitializer): Super(InObjectInitializer),
                                                                       MinValue(0), MaxValue(100), CurrentValue(50)
{
}

void UAttribute::ObjectBeginPlay()
{
	Super::ObjectBeginPlay();
}

void UAttribute::OnRep_MinValue()
{
	OnMaxValueChanged.Broadcast(this, MinValue);
}

void UAttribute::OnRep_MaxValue()
{
	OnMaxValueChanged.Broadcast(this, MaxValue);
}

void UAttribute::OnRep_CurrentValue()
{
	OnValueChanged.Broadcast(this, CurrentValue);
}

UASComponent* UAttribute::GetOwningComponent() const
{
	return GetOwner()->FindComponentByClass<UASComponent>();
}

void UAttribute::OnAttributeListUpdated_Implementation()
{
	// Just handle
}

void UAttribute::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttribute, MinValue, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttribute, MaxValue, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttribute, CurrentValue, Params);
}

void UAttribute::SetMinValue(float InValue)
{
	MinValue = InValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttribute, MinValue, this);
	OnMinValueChanged.Broadcast(this, MinValue);
}

void UAttribute::SetMaxValue(float InValue)
{
	MaxValue = InValue;
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttribute, MaxValue, this);
	OnMaxValueChanged.Broadcast(this, MaxValue);
}

void UAttribute::SetValue(float InValue)
{
	if (InValue < MinValue)
	{
		CurrentValue = MinValue;
		OnValueMinThresholdReached.Broadcast(this, CurrentValue);
	}
	else if (InValue > MaxValue)
	{
		CurrentValue = MaxValue;
		OnValueMaxThresholdReached.Broadcast(this, CurrentValue);
	}
	else
	{
		CurrentValue = InValue;
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(UAttribute, CurrentValue, this);
	OnValueChanged.Broadcast(this, CurrentValue);
}
