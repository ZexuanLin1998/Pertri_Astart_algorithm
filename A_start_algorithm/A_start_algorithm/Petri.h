#include<iostream>
#include<vector>
#include<map>
#include<stack>
#include<memory>
#include<string>
#include<unordered_map>
#include<queue>
using namespace std;
typedef  int WEIGHT;
class Place;
class Transition;
class State;


class State
{
public:
	//std::string name;
	std::vector<int>m;
	std::vector<int>waitting_time;//以等待时间
	bool discard = false;
	int g;
	int h;
	int f;
	vector<shared_ptr<State>> father_Node;
	State() { ; }
	~State()
	{
		;
	}
};


class open_cmp {
public:
	bool operator() (const shared_ptr<State> a, const shared_ptr<State> b) {
		return a->g > b->g;   // 小根堆
	}
	
};

class Petri
{
public:

	std::vector<Place*>placePointer;
	std::vector<Transition*>transPointer;
	std::priority_queue<shared_ptr<State>,vector<shared_ptr<State>>,open_cmp>Open_list;
	//std::priority_queue<shared_ptr<State>>Open_list;
	std::unordered_map<string, list<shared_ptr<State>>>Close_list;
	//std::vector<shared_ptr<State>>Open_list;
	//std::vector<shared_ptr<State>>Close_list;
	Petri() { ; }
	void play(vector<int>M, vector<int>m_Goal, vector<vector<int>>Pre, vector<vector<int>>Post,vector<int>delay, vector<int>goal_place,vector<int>goal_marking,
		vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post);
};

class Vertx
{
public:
	int id;
	std::vector<std::pair<Vertx*, WEIGHT>>preVertx;
	std::vector<std::pair<Vertx*, WEIGHT>>postVertx;
	Vertx() { id = 0; }
	Vertx(int id) :id(id) {
		;
	}
	virtual ~Vertx()
	{
		;
	}
};

class Place :public Vertx
{
public:
	int marking = 0;
	float delay = 0;
	Place() { ; }
	Place(Petri& pn)
	{
		id = pn.placePointer.size();
		pn.placePointer.push_back(this);
	}
	Place(Petri& pn, int id) :Vertx(id) { pn.placePointer.push_back(this); }
	Place(Petri& pn, int id, int marking) :Vertx(id), marking(marking) { pn.placePointer.push_back(this); }
	virtual ~Place()
	{
		;
	}

};

class Transition :public Vertx
{
public:
	Transition(Petri& pn)
	{
		id = pn.transPointer.size();
		pn.transPointer.push_back(this);
	}
	Transition(Petri& pn, int id) :Vertx(id) { pn.transPointer.push_back(this); }
	virtual ~Transition()
	{
		;
	}
};


