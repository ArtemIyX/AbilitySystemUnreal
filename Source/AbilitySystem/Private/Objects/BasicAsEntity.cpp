// © Artem Podorozhko. All Rights Reserved. This project, including all associated assets, code, and content, is the property of Artem Podorozhko. Unauthorized use, distribution, or modification is strictly prohibited.


#include "Objects/BasicAsEntity.h"

UBasicAsEntity::UBasicAsEntity(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FString UBasicAsEntity::GetDebugString_Implementation() const
{
	return FString::Printf(TEXT("As Entity"));
}
