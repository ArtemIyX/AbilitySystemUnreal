// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/Effects/Effect.h"

#include "Components/ASComponent.h"

UEffect::UEffect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bStackable = false;
}

void UEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UEffect::ObjectBeginPlay()
{
	Super::ObjectBeginPlay();
}

void UEffect::EndWork()
{
	if (UASComponent* asComp = GetOwningComponent())
	{
		asComp->EffectHasEnded(this);
	}
	else
	{
		ConditionalBeginDestroy();
	}
}



void UEffect::StartWork_Implementation()
{
	// Some work here
}

void UEffect::OnWorkEnded_Implementation()
{
	// Before deliting
}


bool UEffect::Stack_Implementation(UEffect* AnotherEffect)
{
	/**
	 * If bStackable -> Same effect will call Stack() 
	 */
	return false;
}

void UEffect::OnEffectRemoving_Implementation(UEffect* AnotherEffect)
{
	
}

void UEffect::OnAttributeListUpdated_Implementation()
{
	
}

void UEffect::OnAttributeAdded_Implementation(UAttribute* AnotherAttribute)
{
	
}

void UEffect::OnAttributeRemoving_Implementation(UAttribute* AnotherAttribute)
{
	
}

void UEffect::OnEffectListUpdated_Implementation()
{
}

void UEffect::OnEffectAdded_Implementation(UEffect* AnotherEffect)
{
	
}

UASComponent* UEffect::GetOwningComponent() const
{
	return GetOwner()->FindComponentByClass<UASComponent>();
}

FString UEffect::GetDebugString_Implementation() const
{
	return FString::Printf(TEXT("Effect"));
}

