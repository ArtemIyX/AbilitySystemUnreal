// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASComponent.generated.h"


class UAttribute;
class UEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FASComponentDelegate, UASComponent*, Component);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASComponentAttributeDelegate, UASComponent*, Component, UAttribute*,
                                             Entity);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASComponentEffectDelegate, UASComponent*, Component, UEffect*, Entity);

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

	UPROPERTY(ReplicatedUsing=OnRep_Attributes)
	TArray<UAttribute*> Attributes;

protected:
	UFUNCTION()
	virtual void OnRep_Effects();

	UFUNCTION()
	virtual void OnRep_Attributes();

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
#pragma region Effects
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
	virtual void GetEffectList(TArray<UEffect*>& OutEffects);

#pragma endregion Effects

#pragma region Attributes
	/**
	 * @brief Adds a new attribute of the specified class.
	 *
	 * This function creates and adds an attribute of the specified type.
	 *
	 * @param AttributeClass The class of the attribute to add.
	 * @return A pointer to the newly added attribute.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbilitySystem|Attributes")
	virtual UAttribute* AddAttribute(TSubclassOf<UAttribute> AttributeClass);

	/**
	 * @brief Removes an attribute associated with the specified entity.
	 *
	 * This function removes the specified attribute from the system.
	 *
	 * @param InAttribute The attribute to be removed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbilitySystem|Attributes")
	virtual void RemoveAttributeByEntity(UAttribute* InAttribute);

	/**
	 * @brief Removes all attributes of a specified class.
	 *
	 * This function removes all attributes that are instances of the specified class.
	 *
	 * @param AttributeClass The class of the attributes to remove.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="AbilitySystem|Attributes")
	virtual void RemoveAttributeByClass(TSubclassOf<UAttribute> AttributeClass);

	/**
	 * @brief Retrieves an attribute of the specified class.
	 *
	 * This function checks for the existence of an attribute of the specified class
	 * and outputs it if found.
	 *
	 * @param AttributeClass The class of the attribute to retrieve.
	 * @param OutAttribute The output parameter that will hold the found attribute.
	 * @return True if the attribute was found, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="AbilitySystem|Attributes")
	virtual bool GetAttribute(TSubclassOf<UAttribute> AttributeClass, UAttribute*& OutAttribute) const;

	/**
	 * @brief Gets the attribute of the specified class.
	 *
	 * This function returns the attribute of the specified class.
	 *
	 * @param AttributeClass The class of the attribute to retrieve.
	 * @return A pointer to the attribute of the specified class.
	 */
	virtual UAttribute* Attribute(TSubclassOf<UAttribute> AttributeClass) const;

	/**
	 * @brief Checks if an attribute of the specified class exists.
	 *
	 * This function checks for the presence of an attribute of the specified class.
	 *
	 * @param AttributeClass The class of the attribute to check.
	 * @return True if the attribute exists, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="AbilitySystem|Attributes")
	virtual bool HasAttribute(TSubclassOf<UAttribute> AttributeClass) const;


	/**
	 * @brief Retrieves the list of all active attributes in the component.
	 *
	 * This function populates the provided array with all active attributes within the component.
	 * It is intended for debugging purposes to inspect the current state of attributes.
	 *
	 * @note This function is intended for debugging purposes to inspect the current state of attributes.
	 * 
	 * @param OutAttributes An array to be filled with the currently active attributes.
	 */
	UFUNCTION(BlueprintCallable, Category="AbilitySystem|Attribute")
	virtual void GetAttributeList(TArray<UAttribute*>& OutAttributes);
	
#pragma endregion Attributes

#pragma region Events

	/**
	 * @brief Delegate called when an effect is added.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Effects")
	FASComponentEffectDelegate OnEffectAdded;

	/**
	 * @brief Delegate called when an effect is stacked.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Effects")
	FASComponentEffectDelegate OnEffectStacked;

	/**
	 * @brief Delegate called when an effect is removed.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Effects")
	FASComponentEffectDelegate OnEffectRemoved;

	/**
	 * @brief Delegate called when the effect list is updated.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Effects")
	FASComponentDelegate OnEffectListUpdated;

	/**
	 * @brief Delegate called when an attribute is added.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Attributes")
	FASComponentAttributeDelegate OnAttributeAdded;

	/**
	 * @brief Delegate called when an attribute is removed.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Attributes")
	FASComponentAttributeDelegate OnAttributeRemoved;

	/**
	 * @brief Delegate called when the attribute list is updated.
	 */
	UPROPERTY(BlueprintAssignable, Category="AbilitySystem|Attributes")
	FASComponentDelegate OnAttributeListUpdated;

#pragma endregion Events
};
