/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"

void Prompt_Trigger::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Parent"))
	{
		std::string object = obj["Parent"].GetString();
		parentObjID = GameObjectFactory::GetInstance()->mGameObjectMap[object];
	}
	if (obj.HasMember("ID"))
	{
		id = obj["ID"].GetInt();
	}
	audioPlayed = false;
}

Prompt_Trigger * Prompt_Trigger::Create()
{
	return new Prompt_Trigger();
}
