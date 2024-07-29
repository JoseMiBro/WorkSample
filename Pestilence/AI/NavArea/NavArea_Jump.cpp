#include "Pestilence/AI/NavArea/NavArea_Jump.h"

#include "Pestilence/AI/Enums/ENavAreaFlag.h"

UNavArea_Jump::UNavArea_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);

	DrawColor = FColor::Yellow;
}
