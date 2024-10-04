// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Components/ASComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"


// Sets default values for this component's properties
UASComponent::UASComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UASComponent::OnRep_Effects()
{
}

// Called when the game starts
void UASComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UASComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UASComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(UASComponent, Effects, Params);
}

bool UASComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool sup = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (auto ef : Effects)
	{
		if (IsValid(ef))
		{
			sup |= Channel->ReplicateSubobject(ef, *Bunch, *RepFlags);
			sup |= ef->ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}
	return sup;
}

void UASComponent::EffectHasEnded(UEffect* InEffect)
{
	if (!GetOwner()->HasAuthority())
		return;

	if (!IsValid(InEffect))
		return;

	RemoveEffectByEntity(InEffect);
}

void UASComponent::RemoveEffectByEntity(const UEffect* InEffect)
{
	if (!IsValid(InEffect))
		return;


	// Iterate through the Effects array
	const int32 n = Effects.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UEffect*& ef = Effects[i];

		// Check if the Effect's pointer matches the provided InEffect
		if (IsValid(ef) && InEffect == ef)
		{
			// Notify effect
			ef->OnWorkEnded();

			// Memory cleanup
			ef->ConditionalBeginDestroy();
			ef = nullptr;

			// Remove it from the array
			Effects.RemoveAt(i);
			MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Effects, this);
			return;
		}
	}
}

void UASComponent::RemoveEffectByClass(TSubclassOf<UEffect> EffectClass)
{
	if (!EffectClass)
		return;

	// Iterate through the Effects array
	const int32 n = Effects.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UEffect*& ef = Effects[i];

		// Check if the Effect's class matches the provided EffectClass
		if (IsValid(ef) && ef->IsA(EffectClass))
		{
			// Notify effect
			ef->OnWorkEnded();

			// Memory cleanup
			ef->ConditionalBeginDestroy();
			ef = nullptr;

			// Remove it from the array
			Effects.RemoveAt(i);
			MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Effects, this);
			return;
		}
	}
}

UEffect* UASComponent::AddEffect(TSubclassOf<UEffect> EffectClass)
{
	if (!EffectClass)
		return nullptr;

	UEffect* temp = NewObject<UEffect>(GetOwner(), EffectClass);

	const int32 n = Effects.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UEffect* ef = Effects[i];
		// If we found same classes
		if (IsValid(ef) && ef->IsA(EffectClass))
		{
			// Can we stack?
			if (temp->IsStackable() && ef->IsStackable())
			{
				ef->Stack(temp);
			}

			// If no -> abort 'Add'
			temp->ConditionalBeginDestroy();
			temp = nullptr;
			return nullptr;
		}
	}
	Effects.Add(temp);
	MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Effects, this);
	temp->StartWork();
	return temp;
}

bool UASComponent::GetEffect(TSubclassOf<UEffect> EffectClass, UEffect*& OutEffect) const
{
	OutEffect = Effect(EffectClass);
	return IsValid(OutEffect);
}

UEffect* UASComponent::Effect(TSubclassOf<UEffect> EffectClass) const
{
	// Iterate through the Effects array
	const int32 n = Effects.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UEffect* ef = Effects[i];

		// Check if the Effect's class matches the provided EffectClass
		if (IsValid(ef) && ef->IsA(EffectClass))
		{
			return ef;
		}
	}
	return nullptr;
}

bool UASComponent::HasEffect(TSubclassOf<UEffect> EffectClass) const
{
	UEffect* ef = Effect(EffectClass);
	return IsValid(ef);
}

void UASComponent::GetEffectList(TArray<UEffect*>& OutEffects)
{
	
}
