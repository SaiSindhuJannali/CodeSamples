/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

class Prompt_Trigger :public Component
{
public:
	Prompt_Trigger() :Component(COMP_TRIGGER),parentObjID(NOOBJECT),id(1),audioPlayed(false){}
	~Prompt_Trigger() {}

	virtual void Update() {}
	virtual void Serialize(rapidjson::Value::Object obj);
	virtual void DeSerialize(std::ofstream* file) {}
	virtual void HandleEvent(Event* pEvent) {}

	virtual Prompt_Trigger* Create();
	virtual void Clear() {}

	GameObjectType parentObjID;
	unsigned int id;
	bool audioPlayed;
};
