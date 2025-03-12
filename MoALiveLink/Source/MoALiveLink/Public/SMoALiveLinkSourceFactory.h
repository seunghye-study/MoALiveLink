//======================================================================================================
// Company Name:       MotionTechnologies.inc
// Copyright Year:     2025
// File:               SMoALiveLinkSourceFactory.h
//======================================================================================================

#pragma once

#include "Widgets/SCompoundWidget.h"
#include "Input/Reply.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Types/SlateEnums.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SEditableTextBox;

class MOALIVELINK_API SMoALiveLinkSourceFactory : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnOkClicked, FIPv4Endpoint);

	SLATE_BEGIN_ARGS(SMoALiveLinkSourceFactory) {}
		SLATE_EVENT(FOnOkClicked, OnOkClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void OnEndpointChanged(const FText& NewValue, ETextCommit::Type);
	FReply OnOkClicked();

private:
	TWeakPtr<SEditableTextBox> EditabledText;
	FOnOkClicked OkClicked;
};
