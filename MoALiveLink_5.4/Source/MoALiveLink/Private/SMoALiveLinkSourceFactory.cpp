// Fill out your copyright notice in the Description page of Project Settings.


#include "SMoALiveLinkSourceFactory.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "SlateOptMacros.h"

#define LOCTEXT_NAMESPACE "MoALiveLinkSourceEditor"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMoALiveLinkSourceFactory::Construct(const FArguments& InArgs)
{
	
	FIPv4Endpoint Endpoint;
	Endpoint.Address = FIPv4Address::Any;
	Endpoint.Port = 8000;

	OkClicked = InArgs._OnOkClicked;

	ChildSlot
	[
		// Populate the widget
		SNew(SBox)
			.WidthOverride(250)
			[
				SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.FillWidth(0.5f)
							[
								SNew(STextBlock)
									.Text(LOCTEXT("MoAPortNumber", "PortNumber"))
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Fill)
							.FillWidth(0.5f)
							[
								SAssignNew(EditabledText, SEditableTextBox)
									.Text(FText::FromString(Endpoint.ToString()))
									.OnTextCommitted(this, &SMoALiveLinkSourceFactory::OnEndpointChanged)
							]
					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Right)
					.AutoHeight()
					[
						SNew(SButton)
							.OnClicked(this, &SMoALiveLinkSourceFactory::OnOkClicked)
							[
								SNew(STextBlock)
									.Text(LOCTEXT("Ok", "Ok"))
							]
					]
			]
	];
	
}
void SMoALiveLinkSourceFactory::OnEndpointChanged(const FText& NewValue, ETextCommit::Type)
{
	// ����ڰ� �ؽ�Ʈ�ڽ��� �Է��� ���� ó��
	// weakptr�� ����� �ؽ�Ʈ�ڽ��� ���ϰ� �����ͷ� ����, ��ȿ�� Ȯ��
	TSharedPtr<SEditableTextBox> EditabledTextPin = EditabledText.Pin();
	if (EditabledTextPin.IsValid())
	{
		// ��ȿ���� Ȯ��
		FIPv4Endpoint Endpoint;
		if (!FIPv4Endpoint::Parse(NewValue.ToString(), Endpoint))
		{
			// new value << ����ڰ� �Է��� �ؽ�Ʈ
			// �� �ؽ�Ʈ�� endpoint�� �и� �õ�(ip, port)
			Endpoint.Address = FIPv4Address::Any;
			Endpoint.Port = 8000;
			EditabledTextPin->SetText(FText::FromString(Endpoint.ToString()));
		}
	}
}

FReply SMoALiveLinkSourceFactory::OnOkClicked()
{
	//����ڰ� ok��ư Ŭ�� �� ȣ��
	// �ؽ�Ʈ �ڽ��� �Էµ� ��������Ʈ�� ��������Ʈ�� ����
	TSharedPtr<SEditableTextBox> EditabledTextPin = EditabledText.Pin();
	if (EditabledTextPin.IsValid())
	{
		FIPv4Endpoint Endpoint;
		if (FIPv4Endpoint::Parse(EditabledTextPin->GetText().ToString(), Endpoint))
		{
			OkClicked.ExecuteIfBound(Endpoint);
		}
	}
	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
#undef LOCTEXT_NAMESPACE
