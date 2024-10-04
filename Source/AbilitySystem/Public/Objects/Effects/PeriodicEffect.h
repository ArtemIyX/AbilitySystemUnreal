// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "DurationEffect.h"
#include "PeriodicEffect.generated.h"

/**
 * @class UPeriodicEffect
 * @brief A class representing a duration-based effect that occurs periodically.
 *
 * UPeriodicEffect inherits from UDurationEffect and introduces functionality for applying an effect repeatedly at 
 * specific intervals during its active duration. This class is useful for effects that need to trigger periodically, 
 * such as healing over time or periodic damage, while adhering to the overall duration of the effect.
 */
UCLASS(Abstract)
class ABILITYSYSTEM_API UPeriodicEffect : public UDurationEffect
{
	GENERATED_BODY()
public:
	UPeriodicEffect(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	/**
	* @brief Starts a timer that performs an side action based on the specified duration.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void RunPeriodicTimer();

	/**
	 * @brief Called periodically during the effect's active duration.
	 *
	 * The `PeriodTick` function is executed at regular intervals while the effect is active. It allows the developer
	 * to define custom logic that should occur periodically, such as applying damage, healing, or other time-based effects.
	 * This function is intended to be overridden in derived classes for specific behavior.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void PeriodTick();
public:

	virtual void StartWork_Implementation() override;
	virtual void OnWorkEnded_Implementation() override;
	
	virtual FString GetDebugString_Implementation() const override;

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
