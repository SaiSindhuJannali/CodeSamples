/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "TriggerComponent.h"

TriggerComponent::TriggerComponent():Component(TRIGGERCOMPONENT)
{
	left = right = false;
}

void TriggerComponent::Update()
{

}

void TriggerComponent::DeSerialize(std::ofstream* file)
{

}

void TriggerComponent::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Position")) 
	{
		int temp = obj["Position"].GetInt();

		left = (temp == 0) ? true : false;
		right = (temp == 1) ? true : false;
	}
}

void TriggerComponent::Clear()
{

}

Component* TriggerComponent::Create()
{
	return new TriggerComponent();
}

void TriggerComponent::HandleEvent(Event* pEvent)
{

}

