//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               MoALiveLinkSourceFactory.cpp
//======================================================================================================

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "LiveLinkSourceFactory.h"
#include "MoALiveLinkSourceFactory.generated.h"


UCLASS()
class MOALIVELINK_API UMoALiveLinkSourceFactory : public ULiveLinkSourceFactory
{
	GENERATED_BODY()
public:
	virtual FText GetSourceDisplayName() const override;
	virtual FText GetSourceTooltip() const override;

	virtual EMenuType GetMenuType() const override { return EMenuType::SubPanel; }
	virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;
	TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;

private:
	void OnOkClicked(FIPv4Endpoint Endpoint, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const;
};
