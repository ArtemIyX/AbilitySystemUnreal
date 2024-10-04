// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Objects/Effects/Effect.h"
#include "ASComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName="Ability System Component (Lightweight)")
class ABILITYSYSTEM_API UASComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UASComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Effects)
	TArray<UEffect*> Effects;

protected:
	UFUNCTION()
	virtual void OnRep_Effects();

protected:

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	/* Should be called from effect */
	void EffectHasEnded(UEffect* InEffect);

public:
	/**
	 * @brief Removes an active effect from the component's effect list by its entity.
	 *
	 * This function iterates through the current effects and removes the one that matches
	 * the provided `InEffect` pointer. If a match is found, it triggers the effect's cleanup
	 * and removes it from the array.
	 *
	 * @param InEffect The effect entity to remove from the component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbilitySystem|Effects")
	virtual void RemoveEffectByEntity(const UEffect* InEffect);

	/**
	 * @brief Removes an active effect from the component's effect list by its class type.
	 *
	 * This function removes the first instance of the effect that matches the given class. If a match
	 * is found, the effect is notified, cleaned up, and removed from the component.
	 *
	 * @param EffectClass The class type of the effect to remove.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbilitySystem|Effects")
	virtual void RemoveEffectByClass(TSubclassOf<UEffect> EffectClass);

	/**
	 * @brief Adds an effect to the component's effect list by its class type.
	 *
	 * This function creates a new instance of the provided effect class and adds it to the effect list.
	 * If an effect of the same class already exists, the system checks if it can be stacked. If it can,
	 * the stack operation is performed, otherwise the new effect is discarded.
	 *
	 * @param EffectClass The class type of the effect to add.
	 * @return Effect instance if created successfully
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbilitySystem|Effects")
	virtual UEffect* AddEffect(TSubclassOf<UEffect> EffectClass);

	/**
	 * @brief Retrieves an active effect from the component's effect list by its class type.
	 *
	 * This function searches the effect list and returns the first instance that matches the provided
	 * effect class. If a matching effect is found, it is returned in the `OutEffect` parameter.
	 *
	 * @param OutEffect A reference to store the found effect, if any.
	 * @param EffectClass The class type of the effect to retrieve.
	 * @return True if an effect was found, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="AbilitySystem|Effects")
	virtual bool GetEffect(TSubclassOf<UEffect> EffectClass, UEffect*& OutEffect) const;

	/**
	 * @brief Retrieves an active effect from the component's effect list by its class type.
	 *
	 * This function searches the effect list and returns the first instance that matches the provided
	 * effect class. If a matching effect is found, it is returned in the `OutEffect` parameter.
	 *
	 * @param EffectClass The class type of the effect to retrieve.
	 * @return A pointer to the found effect, if any.
	 */
	virtual UEffect* Effect(TSubclassOf<UEffect> EffectClass) const;

	/**
	 * @brief Checks if the component has an active effect of the specified class.
	 *
	 * This function iterates through the current effect list to determine if an effect of the 
	 * given class exists within the component. Returns true if a matching effect is found.
	 *
	 * @param EffectClass The class type of the effect to check for.
	 * @return True if the component has an effect of the specified class, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="AbilitySystem|Effects")
	virtual bool HasEffect(TSubclassOf<UEffect> EffectClass) const;

	/**
	 * @brief Retrieves the list of all active effects in the component.
	 *
	 * This function populates the provided array with all active effects within the component.
	 * It is intended for debugging purposes to inspect the current state of effects.
	 *
	 * @note This function is intended for debugging purposes to inspect the current state of effects.
	 * 
	 * @param OutEffects An array to be filled with the currently active effects.
	 */
	UFUNCTION(BlueprintCallable, Category="AbilitySystem|Effects")
	void GetEffectList(TArray<UEffect*>& OutEffects);
};
