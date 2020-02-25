/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "SDL_scancode.h"

Prompt::~Prompt(){ }

void Prompt::Update()
{	
	Transform* pTr = static_cast<Transform*>(mpOwner->mParentObject->GetComponent(TRANSFORM));
	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (!flag) {
		pTrans->mPosition.x = pTr->mPosition.x - 160.0f ;
		pTrans->mPosition.y = pTr->mPosition.y + 75.0f;
		//pTrans->mPosition = mOffset * pTr->mScale + pTr->mPosition;
	}



}

void Prompt::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Messages"))
	{
		rapidjson::Value::Array msgs = obj["Messages"].GetArray();
		for (unsigned int i = 0; i < msgs.Size(); ++i)
		{
			PromptData* msg = new PromptData();
			msg->Message = msgs[i]["Message"].GetString();
			if (msgs[i].HasMember("DisplayTime"))
				msg->displayTime = msgs[i]["DisplayTime"].GetInt();
			else
				msg->displayTime = 500;
			if(msgs[i].HasMember("NumJumps"))
				numAllowedJumps = msgs[i]["NumJumps"].GetInt();
			if(msgs[i].HasMember("CollideWith"))
				msg->collideWith = GameObjectFactory::GetInstance()->mGameObjectMap[msgs[i]["CollideWith"].GetString()];
			if (msgs[i].HasMember("ID"))
				msg->id = msgs[i]["ID"].GetInt();
			if (msgs[i].HasMember("Type"))
				msg->Type= msgs[i]["Type"].GetString();
			msg->disable = false;
			message[msg->collideWith].push_back(msg);
			objOccurances[msg->collideWith] = 0;
			disable.push_back(false);
			triggered.push_back(false);
		}
	}
	if (obj.HasMember("Offset"))
	{
		rapidjson::Value::Array offset = obj["Offset"].GetArray();
		mOffset.x = offset[0].GetFloat();
		mOffset.y = offset[1].GetFloat();
	}
}

Prompt* Prompt::Create()
{
	return new Prompt();
}

void Prompt::Clear()
{
}

float Prompt::CurveX(float t)
{
	float x = 0.0f;
	int degree = 4;

	for (int i = 0; i <= degree; ++i)
	{
		x += (float)(cPoints[i][0] * pascalTriangle[i]
			* pow((1 - t), (degree - i)) * pow(t, i));
	}

	return x;
}

float Prompt::CurveY(float t)
{
	int degree = 4;
	float y = 0.0f;
	for (int i = 0; i <= degree; ++i)
	{
		y += (float)(cPoints[i][1] * pascalTriangle[i]
			* pow((1 - t), (degree - i)) * pow(t, i));
	}
	return y;
}

void Prompt::CalculateJump(float &alpha)
{
	float x, y;
	Transform *Enemytrans = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
//	Body *eBody = static_cast<Body *> (mpOwner->GetComponent(BODY));

	if (alpha < 1.0f)
	{
		x = CurveX(alpha);
		Enemytrans->mPosition.x = x;
		//	std::cout << "x = " << x << std::endl;
		y = CurveY(alpha);
		Enemytrans->mPosition.y = y;
		//	std::cout << "y = " << y << std::endl;
		//eBody->Initialize();
		alpha += 0.0128205;
	}
}

void Prompt::calculateStep(float &alpha)
{
	steps = 6 * 6;
	alpha = 1 / steps;
}

void Prompt::ResetFlags()
{
// 	jumpTrigger = false;
// 	goUP = false;
// 	goDOWN = false;
// 	goBUP = false;
// 	goBDOWN = false;
// 	inAIR = false;
	alpha = 0.0f;
	once = 1;
	jump = false;
	flag = false;
}

void Prompt::setFlags(int direction)
{
	switch (direction) {

	case 1:
// 		goDOWN = false;
// 		goBDOWN = false;
// 		goBUP = false;
// 		inAIR = true;
// 		once++;
		break;
	}
}

void Prompt::SetControlPoints(int direction)
{
	Transform* pTr = static_cast<Transform*>(mpOwner->mParentObject->GetComponent(TRANSFORM));
	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	switch (direction) {

	case 1: // go Up
		cPoints[0][0] = pTr->mPosition.x - 400.0f;
		cPoints[0][1] = pTr->mPosition.y + 200.0f;

		cPoints[1][0] = pTr->mPosition.x - 300.0f;
		cPoints[1][1] = pTr->mPosition.y + 350.0f;

		cPoints[2][0] = pTr->mPosition.x - 350.0f;
		cPoints[2][1] = pTr->mPosition.y - 200.0f;

		cPoints[3][0] = pTr->mPosition.x - 160.0f;
		cPoints[3][1] = pTr->mPosition.y + 420.0f;

		cPoints[4][0] = pTr->mPosition.x - 160.0f;
		cPoints[4][1] = pTr->mPosition.y + 75.0f;

		break;
	}
}

void Prompt::HandleEvent(Event* pEvent)
{
	Transform* pTr = static_cast<Transform*>(mpOwner->mParentObject->GetComponent(TRANSFORM));
	Transform* pTrans = (Transform*)mpOwner->GetComponent(TRANSFORM);
	Text* pText = (Text*)mpOwner->mChildObjects[0]->GetComponent(TEXT);
	Body* playerBody = (Body*)mpOwner->mParentObject->GetComponent(BODY);
	std::map<unsigned int, std::vector<PromptData*>>::iterator iter;
	std::vector<PromptData*> msgVector;
	PromptData *pData = nullptr;
	Prompt_Trigger* p_trigger = (Prompt_Trigger*)pEvent->pObject->GetComponent(COMP_TRIGGER);
	GameObjectType objType;
	bool isTriggered = false;
	if (p_trigger)
	{
		id = p_trigger->id - 1;
		isTriggered = triggered[id];
		objType = p_trigger->parentObjID;
	}
	
	if (pEvent->mType == PROMPT_TRIGGER)
	{
		jump = true;
		if (!change) 
		{
			if (pTrans->mAngle <= 12.5f)
			{
				pTrans->mAngle += 0.75f;
			}
			else
				change = true;

		}
		else
		{
			if (pTrans->mAngle >= -12.5f)
			{
				pTrans->mAngle -= 0.75f;
			}
			else
				change = false;

		}

		iter = message.find(objType);
		if (iter != message.end())
		{
			msgVector = iter->second;
			pData = msgVector[id];
			if (disable[id])
			{
				pData->disable = true;
				disable[id] = false;
			}
		}
		triggered[id] = true;
		pos = pTr->mPosition;
		//AudioManager::GetInstance()->setSFXvolume();
	}

	if (pEvent->mType == ONGROUNDCOLLISION)
	{
		if ((pTr->mPosition.x != pos.x))
		{
			pTrans->mScale = glm::vec2(0, 0);
			pText->SetText("");
			//AudioManager::GetInstance()->ResetSFXvolume();
		}
		if ((abs((floor(pTr->mPosition.y) - floor(pos.y))) >= 150) && isTriggered)
		{
			disable[id] = true;
			triggered[id] = false;
		}
	}

	if (pData)
	{
		if (!pData->disable)
		{
			pText->SetText(pData->Message);
			Audio* audio = (Audio*)(pEvent->pObject->GetComponent(AUDIO));

			if (!p_trigger->audioPlayed)
			{
				audio->Play();
				p_trigger->audioPlayed = true;	
			}
			
			Animator* a = static_cast<Animator*>(mpOwner->GetComponent(ANIMATOR));

			if (jump && flag) {
				if (alpha < 1.0f) {
					if (once == 1)
						SetControlPoints(1);
					CalculateJump(alpha);
					setFlags(1);
					audio->Play(0);
				}
				else {
					ResetFlags();
				}
			}

			pTrans->mScale = glm::vec2(100, 50);

			if (pData->displayTime > 0)
			{
				pData->displayTime--;
			}
			else
			{
				pData->disable = true;
			}
		}
		else
		{
			pText->SetText("");
			pTrans->mScale = glm::vec2(0, 0);
		}


		switch (objType)
		{
		case BOX:
			if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_SPACE))
			{
				if ((playerBody->rReturnJumpC() + 1 == numAllowedJumps))
					pData->disable = true;
			}
			break;
		case NUMBOX:
			if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_E) && (pData->Type == "Pick"))
			{
				pData->disable = true;
			}
			if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_A) && (pData->Type == "Add"))
			{
				pData->disable = true;
			}
			if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_S) && (pData->Type == "Sub"))
			{
				pData->disable = true;
			}
			break;
		}
	}
}

glm::vec2 Prompt::calculateOffset(std::string msg)
{
	Transform* pTrans = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	return glm::vec2(-((float)msg.length()/5.0), 0);
}