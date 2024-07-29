// Fill out your copyright notice in the Description page of Project Settings.


#include "Pestilence/Components/AI/PestilenceNavLinkComponent.h"

#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"

UPestilenceNavLinkComponent::UPestilenceNavLinkComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LinkRelativeStart = FVector(0, 0, 0);
	LinkRelativeEnd = FVector(0, 0, 0);

	EnabledAreaClass = UNavArea_Null::StaticClass();
}
