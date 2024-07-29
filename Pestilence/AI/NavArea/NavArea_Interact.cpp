#include "Pestilence/AI/NavArea/NavArea_Interact.h"

#include "NavArea_Jump.h"
#include "Pestilence/AI/Enums/ENavAreaFlag.h"

UNavArea_Interact::UNavArea_Interact(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Interact);

	DrawColor = FColor::White;
}