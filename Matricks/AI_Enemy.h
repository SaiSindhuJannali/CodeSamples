/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified By: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

struct Edge {
	Edge(size_t id1, size_t id2, float weight) :
		id1(id1), id2(id2), weight(weight) {}
	size_t GetID1() const { return id1; }
	size_t GetID2() const { return id2; }
	float  GetWeight() const { return weight; }
private:
	size_t id1;
	size_t id2;
	float weight;
};

struct Vertex {
	Vertex(size_t _id = 0, float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
		: id(_id), x(_x), y(_y), z(_z) {
	}
	size_t ID() const { return id; }
	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }
	bool operator< (const Vertex& rhs) const {
		return id < rhs.id;
	}
private:
	size_t id;
	float x, y, z;
};
/*
template <typename VertexType, typename EdgeType>
class Graph {
public:
	typedef VertexType Vertex;
	typedef EdgeType   Edge;
	////////////////////////////////////////////////////////////
	void InsertEdge(const EdgeType & e) {
		outgoining_edges[GetVertex(e.GetID1())].push_back(e);
	}
	////////////////////////////////////////////////////////////
	void InsertVertex(VertexType v) {
		outgoining_edges.insert(
			std::pair<VertexType, std::vector<EdgeType>>(v, std::vector<EdgeType>())
		);
	}
	////////////////////////////////////////////////////////////
	const VertexType & GetVertex(size_t id) const {
		typename
			std::map< VertexType, std::vector<EdgeType> >::const_iterator it =
			outgoining_edges.find(VertexType(id));
		if (it != outgoining_edges.end()) return it->first;
		else {
			throw BadIndex(id);
		}
	}
	////////////////////////////////////////////////////////////
	//notice return type is a const reference - avoid unnecessary duplication
	const typename std::vector<EdgeType> & GetOutEdges(size_t id) const {
		typename
			std::map< VertexType, std::vector<EdgeType> >::const_iterator it =
			outgoining_edges.find(VertexType(id));
		if (it != outgoining_edges.end()) return it->second;
		else throw BadIndex(id);
	}
	////////////////////////////////////////////////////////////
	//notice return type is a const reference - avoid unnecessary duplication
	const typename std::vector<EdgeType> & GetOutEdges(const VertexType & v) const {
		typename
			std::map< VertexType, std::vector<EdgeType> >::const_iterator it =
			outgoining_edges.find(v);
		if (it != outgoining_edges.end()) return it->second;
		else throw BadIndex(v.ID());
	}
	////////////////////////////////////////////////////////////
	void Print() const {
		typename std::map< VertexType, std::vector<EdgeType> >::const_iterator
			it = outgoining_edges.begin();
		for (; it != outgoining_edges.end(); ++it) {
			std::cout << "Vertex " << it->first.GetName()
				<< " (" << it->first.X() << "," << it->first.Y() << ")" << std::endl;
			size_t size = it->second.size();
			for (size_t i = 0; i < size; ++i) {
				std::cout << "\t" << it->second[i].GetName() << " (" << it->second[i].GetID1()
					<< " -> " << it->second[i].GetID2() << ")" << std::endl;
			}
		}
	}

private:
	// maps vertex to a vector of outgoing edges, 
	// adjacency list
	std::map< VertexType, std::vector<EdgeType> > outgoining_edges;
};
*/

class AIEnemy :public Component
{
public:
	AIEnemy() :Component(AIENEMY), limit(0), active(false) {} //Init(); }
	//void Init();
	void Update();
	void DeSerialize(std::ofstream* file);
	void Serialize(rapidjson::Value::Object obj);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);

	float limit;
	float tartgetX, tartgetY;
	bool active;

};

