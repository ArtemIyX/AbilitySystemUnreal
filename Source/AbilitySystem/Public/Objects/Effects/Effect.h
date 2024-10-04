// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/AdvancedReplicatedObject.h"
#include "Objects/BasicAsEntity.h"
#include "UObject/Object.h"
#include "Effect.generated.h"

class UAttribute;
class UASComponent;
/**
 * @class UEffect
 * @brief Represents an instant effect that must be manually removed.
 * 
 * The UEffect class models an instant effect in the game. This effect 
 * does not automatically expire or get destroyed over time, unlike typical 
 * duration-based effects. Therefore, manual intervention is required to 
 * remove it when its purpose is fulfilled.
 * 
 * This class is network-replicated, and it supports networking features.
 * It provides hooks for beginning and ending the effect via the `Work` 
 * and `WorkEnded` methods, which can be implemented in Blueprints.
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ABILITYSYSTEM_API UEffect : public UBasicAsEntity
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor for UEffect.
	 * 
	 * @param ObjectInitializer Provides initialization for the object.
	 */
	UEffect(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Effect")
	uint8 bStackable : 1;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ObjectBeginPlay() override;
	virtual FString GetDebugString_Implementation() const override;

protected:
	/**
	 * @brief Manually notifies the end of the effect.
	 * 
	 * This function must be called to remove the effect from the system.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void EndWork();

public:
	/**
	* @brief Called when Array of attributes has changed
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Entity|Updates")
	void OnAttributeListUpdated();

	/**
	 * @brief Called when new attribute added
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Entity|Updates")
	void OnAttributeAdded(UAttribute* AnotherAttribute);

	/**
	 * @brief Called when some attribute is being destroyed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Entity|Updates")
	void OnAttributeRemoving(UAttribute* AnotherAttribute);

	/**
	 * @brief Called when Array of effects has changed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Entity|Updates")
	void OnEffectListUpdated();

	/**
	 * @brief Called when new effect added
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Entity|Updates")
	void OnEffectAdded(UEffect* AnotherEffect);

	/**
	 * @brief Called when some effect is being destroyed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Entity|Updates")
	void OnEffectRemoving(UEffect* AnotherEffect);

public:
	/**
	 * @brief The main work logic of the effect.
	 * 
	 * This function is designed to be overridden in Blueprints and represents
	 * the core behavior of the effect.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Effect|Main")
	void StartWork();

	/**
	 * @brief Marks the end of the effect's work.
	 * 
	 * This function is designed to be overridden in Blueprints and is called
	 * when the effect completes its work. However, the effect is not 
	 * automatically removed; it must be removed manually using `NotifyEndWork()`.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Effect|Main")
	void OnWorkEnded();

	/**
	 * @brief Stacks the current effect with another effect.
	 *
	 * This is a BlueprintNativeEvent function that allows stacking 
	 * the current effect with another one. The implementation can be 
	 * provided in both C++ and Blueprints.
	 * @note Another effect will be destroyed after call
	 *
	 * @param AnotherEffect A pointer to the effect that should be stacked with the current one.
	 * @return If stacked successfully true
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Effect|Main")
	bool Stack(UEffect* AnotherEffect);

	/**
	 * @brief Gets Owner Component by class
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Effect|Getters")
	virtual UASComponent* GetOwningComponent() const;

	/**
	 * @brief Checks if the effect is stackable.
	 *
	 * This function determines whether the effect is stackable or not. 
	 * It can be called from both Blueprints and C++ code.
	 *
	 * @return True if the effect is stackable, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Effect|Getters")
	FORCEINLINE bool IsStackable() const { return bStackable; }
};
