/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

struct PromptData
{
	unsigned int id;
	std::string Message;
	glm::vec3 Color;
	int displayTime;
	bool disable;
	unsigned int collideWith;
	std::string Type;
};

class Prompt :public Component
{
public:
	Prompt() :Component(PROMPT), pos(0), disable(0), triggered(0) {
		pascalTriangle[0] = 1;
		pascalTriangle[1] = 4;
		pascalTriangle[2] = 6;
		pascalTriangle[3] = 4;
		pascalTriangle[4] = 1;
		once = 1;
		jump = false;
	}
	~Prompt();
	virtual void Update();

	virtual void Serialize(rapidjson::Value::Object obj);
	virtual void DeSerialize(std::ofstream* file) { }
	virtual void HandleEvent(Event* pEvent);
	glm::vec2 calculateOffset(std::string msg);

	virtual Prompt* Create();
	virtual void Clear();

	float CurveX(float t);
	float CurveY(float t);
	void CalculateJump(float &);
	void calculateStep(float &);
	void ResetFlags();
	void setFlags(int);
	void SetControlPoints(int);


	float cPoints[5][5];
	int pascalTriangle[5];

	float jumpstartPosX, jumpstartPosY, speed,
		rightLimit, leftLimit, playerX, playerY,
		steps, alpha;
	int once;
	bool jump;


	GameObject *player;

private:
	std::map<unsigned int, std::vector<PromptData*>> message;
	std::map<unsigned int, int> objOccurances;
	glm::vec2 mOffset;
	glm::vec2 prevVel;
	glm::vec2 pos;
	std::vector<bool> disable, triggered, audioPlayed;
	int numAllowedJumps,numJumpsOccured;
	int id;
	bool change = false;
	bool flag = true;
	int mTimer = 0;
};