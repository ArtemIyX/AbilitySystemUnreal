// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.

#pragma once

#include "CoreMinimal.h"
#include "Data/AdvancedReplicatedObject.h"
#include "BasicAsEntity.generated.h"

/**
 * @class UBasicAsEntity
 * @brief A base class for entities in the Ability System.
 *
 * This class serves as a base for entities within the ability system. 
 * It is not intended to be used in Blueprints and is designed to 
 * be abstract, providing a foundation for derived classes that will 
 * implement specific entity behaviors and networking functionalities.
 *
 * @note This class inherits from UAdvancedReplicatedObject.
 *
 * @see UAdvancedReplicatedObject
 */
UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class ABILITYSYSTEM_API UBasicAsEntity : public UAdvancedReplicatedObject
{
	GENERATED_BODY()

public:
	UBasicAsEntity(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual bool IsSupportedForNetworking() const override { return true; }

	/**
	 * @brief Retrieves a debug string representation of the object.
	 *
	 * @return A string containing debug information about the object.
	 *
	 * @note This function is intended for debugging purposes only and should not be used in production code.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Entity")
	FString GetDebugString() const;

	
};
