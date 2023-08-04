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
class expand_State;
class open_cmp;
class f_cmp;
class State;


class State
{
public:
	std::vector<int>m;
	std::vector<int>waitting_time;//以等待时间
	bool discard = false;
	//bool discard = true;
	int g = 0;
	int h = 0;
	int f = 0;
	int distance = 0;	
	int tran = 0; 
	bool priorty = false;
	bool open = true;
	vector<shared_ptr<State>> father_Node;
	vector<shared_ptr<State>> childern_Node;
	State() {;}
	~State()
	{
		;
	}
};

class open_cmp {
public:
	virtual bool operator() (const shared_ptr<State> &a, const shared_ptr<State> &b) {
		//return a->h > b->h;
		/*if(a->f==b->f)
			return a->f > b->f; */  // 小根堆//
		//return a->f > b->f;
		return a->g > b->g;
	}
	
};

class expand_State :public State
{
public:
	string m_x_str;
	expand_State() { ; }
};
class f_cmp
{
public:
	bool operator()(const shared_ptr<expand_State>&a, const shared_ptr<expand_State>&b)
	{
		return a->f > b->f;
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
		vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info);
	/*priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>Product_molde(int step,
		vector<int>goal_place, vector<int>goal_marking, vector<int>delays,
		vector<vector<int>> Pre, vector<vector<int>> Post, vector<vector<int>>transpose_Pre,
		vector<vector<int>>transpose_Post,
		vector<vector<string>>place_info);*/
	void Product_molde(vector<int> M, vector<int> m_Goal, vector<vector<int>> Pre, vector<vector<int>> Post, vector<int>delays, vector<int>goal_place, vector<int>goal_marking,
		vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info);
	void Product_molde_new(vector<int> M, vector<int> m_Goal, vector<vector<int>> Pre, vector<vector<int>> Post, vector<int>delays, vector<int>goal_place, vector<int>goal_marking,
		vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info);
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


