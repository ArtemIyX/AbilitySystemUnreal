// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Components/ASComponent.h"

#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Objects/Attribute.h"
#include "Objects/Effects/Effect.h"


// Sets default values for this component's properties
UASComponent::UASComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UASComponent::OnRep_Effects()
{
	OnEffectListUpdated.Broadcast(this);
}

void UASComponent::OnRep_Attributes()
{
	OnAttributeListUpdated.Broadcast(this);
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
	DOREPLIFETIME_WITH_PARAMS_FAST(UASComponent, Attributes, Params);
}

bool UASComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool sup = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (UEffect* entity : Effects)
	{
		if (IsValid(entity))
		{
			sup |= Channel->ReplicateSubobject(entity, *Bunch, *RepFlags);
			sup |= entity->ReplicateSubobjects(Channel, Bunch, RepFlags);
		}
	}

	for (UAttribute* entity : Attributes)
	{
		if (IsValid(entity))
		{
			sup |= Channel->ReplicateSubobject(entity, *Bunch, *RepFlags);
			sup |= entity->ReplicateSubobjects(Channel, Bunch, RepFlags);
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
		UEffect*& entity = Effects[i];

		// Check if the Effect's pointer matches the provided InEffect
		if (IsValid(entity) && InEffect == entity)
		{
			// Notify effect
			entity->OnWorkEnded();

			{
				// Notify all effects except target
				for (int32 j = 0; j < n; ++j)
				{
					if (j != i)
					{
						Effects[j]->OnEffectRemoving(entity);
					}
				}
				// Notify all attributes
				for (int32 k = 0; k < Attributes.Num(); ++k)
				{
					Attributes[k]->OnEffectRemoving(entity);
				}

				OnEffectRemoved.Broadcast(this, entity);
			}


			// Memory cleanup
			entity->ConditionalBeginDestroy();
			entity = nullptr;

			// Remove it from the array
			Effects.RemoveAt(i);
			MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Effects, this);

			{
				for (int32 j = 0; j < Effects.Num(); ++j)
				{
					Effects[j]->OnEffectListUpdated();
				}

				for (int32 j = 0; j < Attributes.Num(); ++j)
				{
					Attributes[j]->OnEffectListUpdated();
				}

				OnEffectListUpdated.Broadcast(this);
			}

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
		UEffect*& entity = Effects[i];

		// Check if the Effect's class matches the provided EffectClass
		if (IsValid(entity) && entity->IsA(EffectClass))
		{
			// Notify effect
			entity->OnWorkEnded();

			{
				// Notify all effects except target
				for (int32 j = 0; j < n; ++j)
				{
					if (j != i)
					{
						Effects[j]->OnEffectRemoving(entity);
					}
				}
				// Notify all attributes
				for (int32 k = 0; k < Attributes.Num(); ++k)
				{
					Attributes[k]->OnEffectRemoving(entity);
				}
				OnEffectRemoved.Broadcast(this, entity);
			}

			// Memory cleanup
			entity->ConditionalBeginDestroy();
			entity = nullptr;

			// Remove it from the array
			Effects.RemoveAt(i);
			MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Effects, this);

			{
				for (int32 j = 0; j < Effects.Num(); ++j)
				{
					Effects[j]->OnEffectListUpdated();
				}

				for (int32 j = 0; j < Attributes.Num(); ++j)
				{
					Attributes[j]->OnEffectListUpdated();
				}
				OnEffectListUpdated.Broadcast(this);
			}

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
		UEffect* entity = Effects[i];
		// If we found same classes
		if (IsValid(entity) && entity->IsA(EffectClass))
		{
			// Can we stack?
			if (temp->IsStackable() && entity->IsStackable())
			{
				if (entity->Stack(temp))
				{
					OnEffectStacked.Broadcast(this, entity);
				}
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
	{
		// Notify all Effects except target (n is previous)
		for (int32 i = 0; i < n; ++i)
		{
			Effects[i]->OnEffectAdded(temp);
			Effects[i]->OnEffectListUpdated();
		}
		// Notify all attributes
		for (int32 k = 0; k < Attributes.Num(); ++k)
		{
			Attributes[k]->OnEffectAdded(temp);
			Attributes[k]->OnEffectListUpdated();
		}
		OnEffectAdded.Broadcast(this, temp);
		OnEffectListUpdated.Broadcast(this);
	}

	return temp;
}

UEffect* UASComponent::GetEffect(TSubclassOf<UEffect> EffectClass)
{
	return Effect(EffectClass);
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
	OutEffects.Empty();
	OutEffects = Effects;
}

UAttribute* UASComponent::AddAttribute(TSubclassOf<UAttribute> AttributeClass)
{
	if (!AttributeClass)
		return nullptr;

	const int32 n = Attributes.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UAttribute* entity = Attributes[i];
		// If we found same classes
		if (IsValid(entity) && entity->IsA(AttributeClass))
		{
			return nullptr;
		}
	}
	UAttribute* temp = NewObject<UAttribute>(GetOwner(), AttributeClass);
	Attributes.Add(temp);
	MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Attributes, this);

	{
		// Notify all Attributes except target (n is previous)
		for (int32 i = 0; i < n; ++i)
		{
			Attributes[i]->OnAttributeAdded(temp);
			Attributes[i]->OnAttributeListUpdated();
		}
		// Notify all effects
		for (int32 k = 0; k < Effects.Num(); ++k)
		{
			Effects[k]->OnAttributeAdded(temp);
			Effects[k]->OnAttributeListUpdated();
		}
		OnAttributeAdded.Broadcast(this, temp);
		OnAttributeListUpdated.Broadcast(this);
	}

	return temp;
}

void UASComponent::RemoveAttributeByEntity(UAttribute* InAttribute)
{
	if (!IsValid(InAttribute))
		return;

	// Iterate through the Attributes array
	int32 n = Attributes.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UAttribute*& entity = Attributes[i];

		// Check if the Effect's pointer matches the provided InAttribute
		if (IsValid(entity) && InAttribute == entity)
		{
			// Notify attribute
			entity->OnWorkEnded();
			// Notify all attributes except target
			{
				for (int32 j = 0; j < n; ++j)
				{
					if (j != i)
					{
						Attributes[j]->OnAttributeRemoving(entity);
					}
				}
				// Notify all effects
				for (int32 k = 0; k < Effects.Num(); ++k)
				{
					Effects[k]->OnAttributeRemoving(entity);
				}
				OnAttributeRemoved.Broadcast(this, entity);
			}


			// Memory cleanup
			entity->ConditionalBeginDestroy();
			entity = nullptr;

			// Remove it from the array
			Attributes.RemoveAt(i);
			MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Attributes, this);
			{
				for (int32 j = 0; j < Effects.Num(); ++j)
				{
					Effects[j]->OnAttributeListUpdated();
				}

				for (int32 j = 0; j < Attributes.Num(); ++j)
				{
					Attributes[j]->OnAttributeListUpdated();
				}
				OnAttributeListUpdated.Broadcast(this);
			}

			return;
		}
	}
}

void UASComponent::RemoveAttributeByClass(TSubclassOf<UAttribute> AttributeClass)
{
	if (!AttributeClass)
		return;

	// Iterate through the Attribute array
	int32 n = Attributes.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UAttribute*& entity = Attributes[i];

		// Check if the Attribute's class matches the provided AttributeClass
		if (IsValid(entity) && entity->IsA(AttributeClass))
		{
			// Notify effect
			entity->OnWorkEnded();

			{
				// Notify all attributes except target
				for (int32 j = 0; j < n; ++j)
				{
					if (j != i)
					{
						Attributes[j]->OnAttributeRemoving(entity);
					}
				}
				// Notify all effects
				for (int32 k = 0; k < Effects.Num(); ++k)
				{
					Effects[k]->OnAttributeRemoving(entity);
				}

				OnAttributeRemoved.Broadcast(this, entity);
			}


			// Memory cleanup
			entity->ConditionalBeginDestroy();
			entity = nullptr;

			// Remove it from the array
			Attributes.RemoveAt(i);
			MARK_PROPERTY_DIRTY_FROM_NAME(UASComponent, Attributes, this);

			{
				for (int32 j = 0; j < Effects.Num(); ++j)
				{
					Effects[j]->OnAttributeListUpdated();
				}

				for (int32 j = 0; j < Attributes.Num(); ++j)
				{
					Attributes[j]->OnAttributeListUpdated();
				}
				OnAttributeListUpdated.Broadcast(this);
			}
			return;
		}
	}
}

UAttribute* UASComponent::GetAttribute(TSubclassOf<UAttribute> AttributeClass)
{
	return Attribute(AttributeClass);
}

UAttribute* UASComponent::Attribute(TSubclassOf<UAttribute> AttributeClass) const
{
	// Iterate through the Effects array
	const int32 n = Attributes.Num();
	for (int32 i = 0; i < n; ++i)
	{
		UAttribute* ef = Attributes[i];

		// Check if the Effect's class matches the provided EffectClass
		if (IsValid(ef) && ef->IsA(AttributeClass))
		{
			return ef;
		}
	}
	return nullptr;
}

bool UASComponent::HasAttribute(TSubclassOf<UAttribute> AttributeClass) const
{
	return IsValid(Attribute(AttributeClass));
}

void UASComponent::GetAttributeList(TArray<UAttribute*>& OutAttributes)
{
	OutAttributes = Attributes;
}
