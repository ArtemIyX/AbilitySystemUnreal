// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "DurationEffect.generated.h"

/**
 * @class UDurationEffect
 * @brief A class representing an effect with a defined duration.
 *
 * UDurationEffect is a type of effect that lasts for a specific amount of time.
 * The effect's behavior is managed by starting a timer, and when the timer ends, the effect concludes.
 */
UCLASS(Abstract)
class ABILITYSYSTEM_API UDurationEffect : public UEffect
{
	GENERATED_BODY()

public:
	UDurationEffect(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void StartWork_Implementation() override;
	virtual bool Stack_Implementation(UEffect* AnotherEffect) override;
	virtual void OnWorkEnded_Implementation() override;

public:
	/**
	* @brief Starts a timer that performs an action based on the specified duration.
	*/
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void RunTimer();

	/**
	* @brief The duration of the effect in seconds.
	*
	* This property can be modified in the editor, and its value is clamped to a minimum of 0.0001 seconds.
	*
	* @note The minimum value is set to 0.0001 seconds.
	*/
	UPROPERTY(BlueprintReadWrite,
		EditDefaultsOnly,
		meta=(UIMin="0.0001",
			ClampMin="0.0001"),
		Category="DurationEffect")
	float Time;

	/**
	* @brief Handle for managing the timer.
	*
	* This property is read-only and is used internally to track the timer's state.
	*/
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle TimerHandle;

	virtual FString GetDebugString_Implementation() const override;
};
