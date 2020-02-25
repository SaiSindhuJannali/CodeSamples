/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
- End Header --------------------------------------------------------*/
#pragma once

#include "Component.h"

class LinerAI : public Component
{
public:
	LinerAI();

	void Update();
	void DeSerialize(std::ofstream* file);
	void Serialize(rapidjson::Value::Object obj);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);
	bool moveRight, moveLeft;
	float rightLimit, leftLimit;
	bool activated;
	float playerX, playerY;
};

