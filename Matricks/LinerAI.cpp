/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
- End Header --------------------------------------------------------*/
#include "stdafx.h"


LinerAI::LinerAI():Component(LINERAI)
{
	moveRight = false;
	moveLeft = true;
	activated = false;
}

void LinerAI::Update()
{

}

void LinerAI::DeSerialize(std::ofstream* file)
{

}

void LinerAI::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("MoveLeft")) {
		moveLeft = obj["MoveLeft"].GetBool();
	}
	if (obj.HasMember("MoveRight")) {
		moveRight = obj["MoveRight"].GetBool();
	}
}

void LinerAI::Clear()
{

}

Component* LinerAI::Create()
{
	return new LinerAI();
}

void LinerAI::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == AIMOVEMENT)
	{
		if (mpOwner->GetType() == ENEMY)
		{
			Transform *trans = static_cast<Transform *> (pEvent->pObject->GetComponent(TRANSFORM));

			rightLimit = trans->mPosition.x + trans->mScale.x / 2;
			leftLimit = trans->mPosition.x - trans->mScale.x / 2;

			Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));
			if (activated)
			{
				if ((playerX - t->mPosition.x) > 0)
				{
					moveRight = true; moveLeft = false;
				}
				else
				{
					moveLeft = true; moveRight = false;
				}
				activated = false;
			}

			if (moveRight)
			{
				t->mPosition.x += 2.5f;
				moveLeft = (t->mPosition.x > rightLimit) ? true : false;
				if (t->mPosition.x > rightLimit)
					t->mPosition.x = rightLimit - 10.0f;
			}

			if (moveLeft)
			{
				t->mPosition.x -= 2.5f;
				moveRight = (t->mPosition.x < leftLimit) ? true : false;
				if (t->mPosition.x < leftLimit)
					t->mPosition.x = leftLimit + 10.0f;
			}

			b->Initialize();
		}

	}
	if (pEvent->mType == L_AICOLLISION)
	{
		activated = true;
		Transform *trans = static_cast<Transform *> (pEvent->pObject->GetComponent(TRANSFORM));
		Transform *Enemytrans = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));

		playerX = trans->mPosition.x;
		playerY = trans->mPosition.y;

		if (abs(playerX - Enemytrans->mPosition.x) <= 70.0f)
		{
			/*std::cout << "DEAD : " << std::endl;*/
			((Audio*)mpOwner->GetComponent(AUDIO))->Play(0);
			GameStateManager::GetInstance()->ChangeLevel(LEVEL2);

		}
		else if (abs(Enemytrans->mPosition.x - playerX) <= 70.0f)
		{
			/*std::cout << "DEAD : " << std::endl;*/
			((Audio*)mpOwner->GetComponent(AUDIO))->Play(0);
			GameStateManager::GetInstance()->ChangeLevel(LEVEL2);
		}
	}

}



