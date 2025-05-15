// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "PeriodicInstantEffect.generated.h"

/**
 * @class UPeriodicInstantEffect
 * @brief A class representing an effect with Ticking action
 *
* @note Instant (you should remove it by hand)
 */
UCLASS(Abstract)
class ABILITYSYSTEM_API UPeriodicInstantEffect : public UEffect
{
	GENERATED_BODY()

public:
	UPeriodicInstantEffect(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	/**
	* @brief Starts a timer that performs an side action based on the specified duration.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void RunPeriodicTimer();

	/**
	* @brief Clears a timer that performs an periodic tick action
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void ClearPeriodicTimer();
	
	/**
	 * @brief Called periodically
	 *
	 * The `PeriodTick` function is executed at regular intervals while the effect is active. It allows the developer
	 * to define custom logic that should occur periodically, such as applying damage, healing, or other time-based effects.
	 * This function is intended to be overridden in derived classes for specific behavior.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void PeriodTick();
public:

	virtual void StartWork_Implementation() override;
	virtual FString GetDebugString_Implementation() const override;
	virtual void EndWork() override;
	
	/**
	* @brief The period of the side effect in seconds.
	*
	* @note The minimum value is set to 0.0001 seconds.
	*/
	UPROPERTY(BlueprintReadWrite,
		EditDefaultsOnly,
		meta=(UIMin="0.0001",
			ClampMin="0.0001"),
		Category="PeriodicEffect")
	float Period;

	
	/**
	* @brief Handle for managing the periodic timer.
	*
	* This property is read-only and is used internally to track the timer's state.
	*/
	FTimerHandle PeriodicTimerHandle;
};
