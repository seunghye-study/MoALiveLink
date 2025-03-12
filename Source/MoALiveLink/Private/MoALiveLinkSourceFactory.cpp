//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               MoALiveLinkSourceFactory.cpp
//======================================================================================================



#include "MoALiveLinkSourceFactory.h"
#include "MoALiveLinkSource.h"
#include "SMoALiveLinkSourceFactory.h"

#define LOCTEXT_NAMESPACE "MoALiveLinkSource"

FText UMoALiveLinkSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("SourceDisplayName", "MoA LiveLink");
}

FText UMoALiveLinkSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("SourceTooltip", "Creates a connection to MoA Stream");
}

TSharedPtr<SWidget> UMoALiveLinkSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const
{
	return SNew(SMoALiveLinkSourceFactory)
		.OnOkClicked(SMoALiveLinkSourceFactory::FOnOkClicked::CreateUObject(this, &UMoALiveLinkSourceFactory::OnOkClicked, OnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> UMoALiveLinkSourceFactory::CreateSource(const FString& ConnectionString) const
{
	FIPv4Endpoint DeviceEndPoint;
	UE_LOG(LogTemp, Log, TEXT("CreateSource Called"));
	if (!FIPv4Endpoint::Parse(ConnectionString, DeviceEndPoint))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ConnectionString: %s"), *ConnectionString);
		return TSharedPtr<ILiveLinkSource>();
	}
	UE_LOG(LogTemp, Log, TEXT("Parsed IP: %s, Port: %d"), *DeviceEndPoint.Address.ToString(), DeviceEndPoint.Port);
	return MakeShared<MoALiveLinkSource>(DeviceEndPoint);
}

void UMoALiveLinkSourceFactory::OnOkClicked(FIPv4Endpoint Endpoint, FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const
{
	// OnClick Event Deligate Handler
	OnLiveLinkSourceCreated.ExecuteIfBound(MakeShared<MoALiveLinkSource>(Endpoint), Endpoint.ToString());
}

#undef LOCTEXT_NAMESPACE