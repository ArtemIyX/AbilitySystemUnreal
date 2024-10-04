// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "BasicAsEntity.h"
#include "Data/AdvancedReplicatedObject.h"

#include "UObject/Object.h"
#include "Attribute.generated.h"

class UEffect;
class UASComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeValueDelegate, UAttribute*, AttributePtr, float, Value);

/**
 * @class UAttribute
 * @brief A class that represents an attribute with a minimum, maximum, and current value, which can be replicated.
 * @note Enable Unreal Push Model: https://www.kierannewland.co.uk/push-model-networking-unreal-engine/
 * 
 * This class provides the ability to set and get the values of an attribute, with replication support for 
 * network synchronization. It also includes delegate events to notify listeners when the attribute values change.
 */
UCLASS(Blueprintable, BlueprintType)
class ABILITYSYSTEM_API UAttribute : public UBasicAsEntity
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructs a UAttribute object.
	 *
	 * @param InObjectInitializer The object initializer used to initialize this object. 
	 *                            Defaults to FObjectInitializer::Get().
	 */
	UAttribute(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

protected:
	/**
	 * @brief Minimum value of the attribute.
	 * 
	 * This value is replicated and is only editable in the defaults. Changes are broadcast via the OnMinValueChanged delegate.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attribute|Defaults", ReplicatedUsing=OnRep_MinValue)
	float MinValue;

	/**
	 * @brief Maximum value of the attribute.
	 * 
	 * This value is replicated and is only editable in the defaults. Changes are broadcast via the OnMaxValueChanged delegate.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attribute|Defaults", ReplicatedUsing=OnRep_MaxValue)
	float MaxValue;

	/**
	 * @brief Current value of the attribute.
	 * 
	 * This value is replicated and changes are broadcast via the OnValueChanged delegate.
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Attribute|Current", ReplicatedUsing=OnRep_CurrentValue)
	float CurrentValue;

protected:
	virtual void ObjectBeginPlay() override;
	/**
	 * @brief Called when MinValue is replicated.
	 * 
	 * This function is automatically triggered when the MinValue is updated on the client side.
	 */
	UFUNCTION()
	virtual void OnRep_MinValue();

	/**
	 * @brief Called when MaxValue is replicated.
	 * 
	 * This function is automatically triggered when the MaxValue is updated on the client side.
	 */
	UFUNCTION()
	virtual void OnRep_MaxValue();

	/**
	 * @brief Called when CurrentValue is replicated.
	 * 
	 * This function is automatically triggered when the CurrentValue is updated on the client side.
	 */
	UFUNCTION()
	virtual void OnRep_CurrentValue();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual FString GetDebugString_Implementation() const override;

public:
	/**
	 * @brief Marks the end of the attribute's work.
	 * 
	 * This function is designed to be overridden in Blueprints and is called
	 * when the attribute is being destroyed.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Attribute")
	void OnWorkEnded();

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
	 * @brief Gets Owner Component by class
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attribute|Getters")
	UASComponent* GetOwningComponent() const;

	/**
	 * @brief Current / Max
	 * @return Value ratio (0.0 - 1.0)
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attribute|Getters")
	float GetValuePercent() const;

	/**
	 * @brief Gets the minimum value of the attribute.
	 * @return The current minimum value.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attribute|Getters")
	FORCEINLINE float GetMinValue() const { return MinValue; }

	/**
	 * @brief Gets the maximum value of the attribute.
	 * @return The current maximum value.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attribute|Getters")
	FORCEINLINE float GetMaxValue() const { return MaxValue; }

	/**
	 * @brief Gets the current value of the attribute.
	 * @return The current value.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Attribute|Getters")
	FORCEINLINE float GetCurrentValue() const { return CurrentValue; }

	/**
	 * @brief Sets the minimum value of the attribute.
	 * 
	 * Only callable on the server. Broadcasts the OnMinValueChanged delegate.
	 * 
	 * @param InValue The new minimum value to set. Defaults to 0.0f.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Attribute|Setters")
	virtual void SetMinValue(float InValue = 0.0f);

	/**
	 * @brief Sets the maximum value of the attribute.
	 * 
	 * Only callable on the server. Broadcasts the OnMaxValueChanged delegate.
	 * 
	 * @param InValue The new maximum value to set. Defaults to 100.0f.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Attribute|Setters")
	virtual void SetMaxValue(float InValue = 100.0f);

	/**
	 * @brief Sets the current value of the attribute.
	 * 
	 * Ensures that the value is clamped between MinValue and MaxValue. Broadcasts the OnValueChanged delegate, 
	 * and triggers threshold events if the value hits MinValue or MaxValue.
	 * 
	 * @param InValue The new value to set. Defaults to 50.0f.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Attribute|Setters")
	virtual void SetValue(float InValue = 50.0f);

	/**
	 * @brief Delegate for notifying when MinValue has changed.
	 * 
	 * This delegate is broadcast whenever the MinValue is updated.
	 */
	UPROPERTY(BlueprintAssignable, Category="Attribute|Events")
	FAttributeValueDelegate OnMinValueChanged;

	/**
	 * @brief Delegate for notifying when MaxValue has changed.
	 * 
	 * This delegate is broadcast whenever the MaxValue is updated.
	 */
	UPROPERTY(BlueprintAssignable, Category="Attribute|Events")
	FAttributeValueDelegate OnMaxValueChanged;

	/**
	 * @brief Delegate for notifying when CurrentValue has changed.
	 * 
	 * This delegate is broadcast whenever the CurrentValue is updated.
	 */
	UPROPERTY(BlueprintAssignable, Category="Attribute|Events")
	FAttributeValueDelegate OnValueChanged;

	/**
	 * @brief Delegate for notifying when CurrentValue reaches the MinValue threshold.
	 * 
	 * This delegate is broadcast when the CurrentValue is clamped to MinValue.
	 */
	UPROPERTY(BlueprintAssignable, Category="Attribute|Events")
	FAttributeValueDelegate OnValueMinThresholdReached;

	/**
	 * @brief Delegate for notifying when CurrentValue reaches the MaxValue threshold.
	 * 
	 * This delegate is broadcast when the CurrentValue is clamped to MaxValue.
	 */
	UPROPERTY(BlueprintAssignable, Category="Attribute|Events")
	FAttributeValueDelegate OnValueMaxThresholdReached;
};
