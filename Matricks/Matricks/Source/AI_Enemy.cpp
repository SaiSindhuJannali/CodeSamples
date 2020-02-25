/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified By: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include"stdafx.h"

/*void AIEnemy::Init()
{
	enum V { A, B, C, D, E, F, G };

	//make sure algorithm works with all permutation of edges AB AC AD
	//that is correctness doesn't depend on tie-breaking
	std::vector<int> order;
	order.push_back(1);
	order.push_back(2);
	order.push_back(3);
	float worst_solution = 0;
	//long worst_time = 0;
	do {
		//std::cout << order[0] << " "  << order[1] << " " << order[2] << std::endl;
		typedef Graph< Vertex, Edge >  GraphType;
		//typedef Astar< GraphType, Heuristic<GraphType> >  AstarSearchType;
		//typedef Callback< GraphType, AstarSearchType >        CallbackType;

		GraphType graph;
		graph.InsertVertex(GraphType::Vertex(A));
		graph.InsertVertex(GraphType::Vertex(B));
		graph.InsertVertex(GraphType::Vertex(C));
		graph.InsertVertex(GraphType::Vertex(D));
		graph.InsertVertex(GraphType::Vertex(E));
		graph.InsertVertex(GraphType::Vertex(F));
		graph.InsertEdge(GraphType::Edge(A, static_cast<V>(order[0]), 1));
		graph.InsertEdge(GraphType::Edge(static_cast<V>(order[0]), A, 1));
		graph.InsertEdge(GraphType::Edge(A, static_cast<V>(order[1]), 1));
		graph.InsertEdge(GraphType::Edge(static_cast<V>(order[1]), A, 1));
		graph.InsertEdge(GraphType::Edge(A, static_cast<V>(order[2]), 1));
		graph.InsertEdge(GraphType::Edge(static_cast<V>(order[2]), A, 1));
		graph.InsertEdge(GraphType::Edge(B, E, 1));
		graph.InsertEdge(GraphType::Edge(E, B, 1));
		graph.InsertEdge(GraphType::Edge(C, E, 2));
		graph.InsertEdge(GraphType::Edge(E, C, 2));
		graph.InsertEdge(GraphType::Edge(D, E, 3));
		graph.InsertEdge(GraphType::Edge(E, D, 3));
		graph.InsertEdge(GraphType::Edge(E, F, 5));
		graph.InsertEdge(GraphType::Edge(F, E, 5));
}*/

void AIEnemy::Update()
{
	if (!active)
	{
		//check the light of sight

	}
}

void AIEnemy::DeSerialize(std::ofstream* file)
{}

void AIEnemy::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("bound")) {
		limit = obj["bound"].GetFloat();
	}
	//std::cout << limit;
}

void AIEnemy::Clear()
{

}

Component* AIEnemy::Create()
{
	return new AIEnemy();
}

void AIEnemy::HandleEvent(Event* pEvent)
{

}