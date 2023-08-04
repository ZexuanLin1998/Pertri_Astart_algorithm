#include "Petri.h"
#include<memory>
#include<algorithm>
#include <iostream>
#include<time.h>
#include<random>
bool compare_G(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g);
inline bool compare_V_son(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g);
inline bool compare_V(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g);
inline bool compare_list_and_CurNode(unordered_map<string, list<shared_ptr<State>>>Open_list, pair<string, shared_ptr<State>>curNode_Son);
bool compare(shared_ptr<State>data1, shared_ptr<State>data2);
inline void seek_childern_Son(priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>&open_list, shared_ptr<State>Son);
void Petri::play(vector<int> M, vector<int> m_Goal, vector<vector<int>> Pre, vector<vector<int>> Post,vector<int>delays,vector<int>goal_place,vector<int>goal_marking,
	vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info)
{
	/****************记录时间***********************************************/
	clock_t total_start, total_end;
	total_start = clock();
	shared_ptr<State> inital_temp(new State);
	/*提取初始状态*/
	for (int i = 0;i < M.size();i++)
		inital_temp->m.push_back(M[i]);
	for (int i = 0;i < delays.size();i++)
		inital_temp->waitting_time.push_back(0);
	inital_temp->g = 0;
	inital_temp->f = 0;
	for (int i = 0;i < inital_temp->m.size();i++)
	{
		if (inital_temp->m[i] > 0)
		{
			if (place_info[i][2] == "initial")
			{
				inital_temp->distance = inital_temp->distance + inital_temp->m[i] * atoi(place_info[i][1].c_str());
			}
			inital_temp->distance = inital_temp->distance + delays[i];
		}
			
	}
	inital_temp->f = inital_temp->g + inital_temp->distance;
	vector<string>init_m_info;//初始标识信息
	for (int i = 0;i < inital_temp->m.size();i++)
	{
		if (inital_temp->m[i] > 0)
			init_m_info.push_back(to_string(i).append("/").append(to_string(inital_temp->m[i])).append("/"));
	}
	string init_str;//定义个字符串将初始标识的库所id和token个数进行拼接
	for (int i = 0;i < init_m_info.size();i++)
	{
		init_str.append(init_m_info[i]);
	}
	inital_temp->discard = false;
	inital_temp->priorty = true;
	Open_list.emplace(inital_temp);
	int count = 0;
	int num = 0;
	while (!Open_list.empty())
	{
		vector<int>enable_tran;
		shared_ptr<State> curNode(new State);
		/*如果当前结点是旧结点直接删除*/
		while (Open_list.top()->discard == true)
		{
			Open_list.pop();
			//continue;
		}
		/**************************************************************************/
		auto list = Open_list.top();
		curNode->m = list->m;
		curNode->waitting_time = list->waitting_time;
		curNode->g = list->g;
		curNode->discard = list->discard;
		curNode->distance = list->distance;
		curNode->h = list->h;
		curNode->f = list->f;
		curNode->tran = list->tran;
		curNode->father_Node = list->father_Node;
		curNode->priorty = list->priorty;
		
		//cout << "f =" << curNode->f << " ";
		//cout << "g =" << curNode->g << " ";
		//cout << " h =" << curNode->h << " ";
		//cout << endl;
		//for (int i=0;i<curNode->m.size();i++)
		//	cout <<"p["<< i+1 <<"] = " << curNode->m[i] << " ";
		//cout << endl;
		//cout << "g = " << curNode->g << endl;
		//cout << endl				
		//if (Open_list.top()->priorty == false)
		//{
		//	Open_list.pop();
		//	continue;
		//}		
		if (curNode->m[goal_place[0]] == goal_marking[0])
		{
			/**********************输出时间***********************************************/
			total_end = clock();
			cout << "total_time = " << double(total_end - total_start) / CLOCKS_PER_SEC << "s" << endl;
			cout << "num = " << num << endl;
			cout << "average_time = " << double(total_end - total_start) / CLOCKS_PER_SEC / num << "s" << endl;
			//exit(1);
			cout << "f = " << curNode->f << " ";
			cout << "g = " << curNode->g << " ";
			cout << "h =" << curNode->h << " ";
			cout << endl;
			/*for (auto it : curNode->waitting_time)
				cout << it <<" ";
			cout << endl;*/
			cout << Close_list.size() << " " << endl;
			while (!curNode->father_Node.empty())
			{
				int num = curNode->tran;
				cout << "t" << num+1 << " ";
				curNode = curNode->father_Node[0];

			}
			cout << endl;
		}


		/*************************************************************************/
		vector<string>curNode_m_info;//已扩展节点的标识信息
		for (int i = 0;i < curNode->m.size();i++)
		{
			if (curNode->m[i] > 0)
				curNode_m_info.push_back(to_string(i).append("/").append(to_string(curNode->m[i])).append("/"));
		}
		string curNode_str; //定义个字符串将已扩展节点标识的库所id和token个数进行拼接
		for (int i = 0;i < curNode_m_info.size();i++)
		{
			curNode_str.append(curNode_m_info[i]);
		}
		/*************************************************************************/

		Open_list.pop();
			
		for (int i = 0;i < Pre[0].size();i++)
		{
			count = 0;
			for (int j = 0;j < Pre.size();j++)
				if (curNode->m[j] >= Pre[j][i])
					count++;
			if (count >= Pre.size())
			{
				count = 0;
				enable_tran.push_back(i);
			}
		}
		
		for (auto t : enable_tran)
		{
			shared_ptr<State>curNode_Son(new State);
			vector<int> lamba;
			vector<int>unempty_place;
			for (int i = 0;i < transpose_Pre[t].size();i++)
			{
				if (transpose_Pre[t][i] > 0)
					unempty_place.push_back(i);
			}
			for (int i=0;i< unempty_place.size();i++)
			{
				lamba.push_back(delays[unempty_place[i]] - curNode->waitting_time[unempty_place[i]]);
			}
			int max_lamba = *max_element(lamba.begin(),lamba.end());
			//cout << max_lamba << " " << endl;
			if (max_lamba < 0)
				max_lamba = 0;
			for (int p = 0;p < Pre.size();p++)
			{
				curNode_Son->m.push_back(curNode->m[p] + Post[p][t] - Pre[p][t]);
				curNode_Son->waitting_time.push_back(curNode->waitting_time[p] - curNode->waitting_time[p] * Pre[p][t] + max_lamba * (curNode_Son->m[p] - Post[p][t]));	
				if (curNode_Son->waitting_time[p] < 0)
					curNode_Son->waitting_time[p] = 0;
				if (delays[p] == 0)
					curNode_Son->waitting_time[p] = 0;
			}
			curNode_Son->g = curNode->g + max_lamba;
			vector<string>curNode_Son_m_info;//待扩展节点的标识信息
			vector<int>Son_unempty_place;
			for (int i = 0;i < curNode_Son->m.size();i++)
			{
				if (curNode_Son->m[i] > 0)
				{
					Son_unempty_place.push_back(i);

					curNode_Son_m_info.push_back(to_string(i).append("/").append(to_string(curNode_Son->m[i])).append("/"));
				}
			}
			string curNode_Son_str;//待扩展节点含有token的库所以及个数进行拼接
			for (int i = 0;i < curNode_Son_m_info.size();i++)
			{
				curNode_Son_str.append(curNode_Son_m_info[i]);
			}
			curNode_Son->father_Node.push_back(list);
			curNode_Son->tran = t;
			/******************************************************************************************/
			//vector<int>max_value;
			priority_queue<int>max_value;
			//for (int i = 0;i < curNode_Son->m.size();i++)
			//{
			//	if (curNode_Son->m[i] > 0)
			//	{
			//		
			//		max_value.push_back(curNode_Son->m[i] * atoi(place_info[i][1].c_str()));
			//	}
			//}
			for (int i = 0;i < Son_unempty_place.size();i++)
			{
				max_value.push(curNode_Son->m[i] * atoi(place_info[i][1].c_str()));
				//max_value.push_back(curNode_Son->m[i] * atoi(place_info[i][1].c_str()));
			}
			curNode_Son->h = max_value.top();
			//curNode_Son->h = curNode_Son->distance;
			//curNode_Son->h = 40;
			curNode_Son->f = curNode_Son->g + curNode_Son->h;
			/*将已扩展节点存入close_list中*/
			std::list<shared_ptr<State>> member_list;
			member_list.push_back(curNode);
			Close_list.emplace(make_pair(curNode_str, member_list));
			if (Close_list.find(curNode_Son_str) == Close_list.end())
			{	
				std::list<shared_ptr<State>>curNode_member;
				Open_list.emplace(curNode_Son);
				curNode_member.push_back(curNode_Son);
				Close_list.emplace(make_pair(curNode_Son_str,curNode_member));
			}
			else
			{
				auto c_list = Close_list[curNode_Son_str];
				for (auto it : c_list)
				{
					//if (curNode_Son->g < it->g)
					//{
					//	it->discard = true;
					//	Close_list[curNode_Son_str].remove(it);
					//	Open_list.emplace(curNode_Son);
					//	Close_list[curNode_Son_str].push_back(curNode_Son);
					//}
					//if (curNode_Son->g < it->g || !compare_V(it,curNode_Son))
					//{	
					//	if (curNode_Son->g >= it->g && compare_V(curNode_Son, it)==true)
					//	{
					//		it->discard = true;	
					//		
					//	}
					//	//it->discard = true;
					//	Close_list[curNode_Son_str].push_back(curNode_Son);
					//	Close_list[curNode_Son_str].remove(it);
					//	Open_list.emplace(curNode_Son);
					//}
					if (!compare_G(it, curNode_Son))
					{
						if (compare_G(curNode_Son, it))
						{
							it->discard = true;	
							Close_list[curNode_Son_str].remove(it);
							Close_list[curNode_Son_str].push_back(curNode_Son);
							Open_list.emplace(curNode_Son);
						}
						else
						{
							Open_list.emplace(curNode_Son);
						}
						
					}
				}
			}			
		}

		num++;
	}
	cout << " 已扩展结点" <<Close_list.size() << endl;

}

void Petri::Product_molde(vector<int> M, vector<int> m_Goal, vector<vector<int>> Pre, vector<vector<int>> Post, vector<int>delays, vector<int>goal_place, vector<int>goal_marking,
	vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info)
{
	/******************************记录时间、次数****************************************************/
	clock_t total_start, total_end;
	total_start = clock();
	int time_count = 0;
	//cin >> step;
	shared_ptr<State> inital_temp(new State);
	/*提取初始状态*/
	for (int i = 0;i < M.size();i++)
		inital_temp->m.push_back(M[i]);
	for (int i = 0;i < delays.size();i++)
		inital_temp->waitting_time.push_back(0);
	inital_temp->g = 0;
	inital_temp->f = 0;
	for (int i = 0;i < inital_temp->m.size();i++)
	{
		if (inital_temp->m[i] > 0)
		{
			inital_temp->distance = inital_temp->distance + inital_temp->m[i] * atoi(place_info[i][1].c_str());
		}

	}
	inital_temp->h = inital_temp->distance;
	inital_temp->f = inital_temp->g + inital_temp->h;
	Open_list.emplace(inital_temp);
	int count = 0;
	int total_time = 0;
	auto root = inital_temp;
	int step_num = 0;
	
	while (!Open_list.empty())
	{
		priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>best_Son_node;
		unordered_map<string, list<shared_ptr<State>>>visited_nodes;
		vector<shared_ptr<State>>Open_list_temp;
		int step = 10- step_num;//设置步长						
		/*************************将open表中的members存入临时容器Open_list_temp*********************************************/
		/***************************************************/
		vector<string>root_m_info;//初始标识信息
		for (int i = 0;i < Open_list.top()->m.size();i++)
		{
			if (Open_list.top()->m[i] > 0)
				root_m_info.push_back(to_string(i).append("/").append(to_string(Open_list.top()->m[i])).append("/"));
		}
		string root_str;//定义个字符串将初始标识的库所id和token个数进行拼接
		for (int i = 0;i < root_m_info.size();i++)
		{
			root_str.append(root_m_info[i]);
		}
		/************设置一个临时open_list按顺序遍历**********/
		Open_list_temp.push_back(Open_list.top());
		std::list<shared_ptr<State>> member_list;
		member_list.push_back(Open_list.top());
		visited_nodes.emplace(make_pair(root_str, member_list));
		if (Open_list.top()->m[goal_place[0]] == goal_marking[0])
		{
			cout << "f = " << Open_list.top()->f << " ";
			cout << "g = " << Open_list.top()->g << " ";
			cout << "h =" << Open_list.top()->h << " ";
			cout << endl;
			auto _root = Open_list.top();
			while (!_root->father_Node.empty())
			{
				int num = _root->tran;
				cout << "t" << num + 1 << " ";
				_root = _root->father_Node[0];
			}
			cout << endl;
		}
		Open_list.pop();
		int open_list_size = Open_list_temp.size();
		clock_t start, end;
		start = clock();
		time_count++;
		while (step != 0 && !Open_list_temp.empty())
		{		
			vector<int>enable_tran;
			priority_queue<shared_ptr<expand_State>, vector<shared_ptr<expand_State>>, f_cmp> best_f_list;
			shared_ptr<State>CurNode(new State);
			if (Open_list_temp[0]->discard == true)
			{
				Open_list_temp.erase(Open_list_temp.begin());
				continue;
			}
			auto list = Open_list_temp[0];
			CurNode = list;
			/*************************************************************************/
			vector<string>CurNode_m_info;//已扩展节点的标识信息
			for (int i = 0;i < CurNode->m.size();i++)
			{
				if (CurNode->m[i] > 0)
					CurNode_m_info.push_back(to_string(i).append("/").append(to_string(CurNode->m[i])).append("/"));
			}
			string CurNode_str; //定义个字符串将已扩展节点标识的库所id和token个数进行拼接
			for (int i = 0;i < CurNode_m_info.size();i++)
			{
				CurNode_str.append(CurNode_m_info[i]);
			}
			/*************************************************************************/
			Open_list_temp.erase(Open_list_temp.begin());
			/***************************寻找使能变迁***********************************/
			int count = 0;
			for (int i = 0;i < Pre[0].size();i++)
			{
				count = 0;
				for (int j = 0;j < Pre.size();j++)
					if (CurNode->m[j] >= Pre[j][i])
						count++;
				if (count >= Pre.size())
				{
					count = 0;
					enable_tran.push_back(i);
				}
			}
			/********************使能变迁激发******************************************/
			for (auto t : enable_tran)
			{
				/********************计算lamba*****************************/
				shared_ptr<expand_State>CurNode_Son(new expand_State);
				vector<int> lamba;
				vector<int>unempty_place;
				for (int i = 0;i < transpose_Pre[t].size();i++)
				{
					if (transpose_Pre[t][i] > 0)
						unempty_place.push_back(i);
				}
				for (int i = 0;i < unempty_place.size();i++)
				{
					lamba.push_back(delays[unempty_place[i]] - CurNode->waitting_time[unempty_place[i]]);

				}
				int max_lamba = *max_element(lamba.begin(), lamba.end());
				if (max_lamba < 0)
					max_lamba = 0;
				/***********************计算m、已等待时间、g**********************************/
				for (int p = 0;p < Pre.size();p++)
				{
					CurNode_Son->m.push_back(CurNode->m[p] + Post[p][t] - Pre[p][t]);
					CurNode_Son->waitting_time.push_back(CurNode->waitting_time[p] - CurNode->waitting_time[p] * Pre[p][t] + max_lamba * (CurNode_Son->m[p] - Post[p][t]));
					if (CurNode_Son->waitting_time[p] < 0)
						CurNode_Son->waitting_time[p] = 0;
					if (delays[p] == 0)
						CurNode_Son->waitting_time[p] = 0;
				}
				CurNode_Son->g = CurNode->g + max_lamba;
				/***********************拼接子节点信息**************************************************/
				vector<string>CurNode_Son_m_info;//待扩展节点的标识信息
				for (int i = 0;i < CurNode_Son->m.size();i++)
				{
					if (CurNode_Son->m[i] > 0)
						CurNode_Son_m_info.push_back(to_string(i).append("/").append(to_string(CurNode_Son->m[i])).append("/"));
				}
				string CurNode_Son_str;//待扩展节点含有token的库所以及个数进行拼接
				for (int i = 0;i < CurNode_Son_m_info.size();i++)
				{
					CurNode_Son_str.append(CurNode_Son_m_info[i]);
				}
				CurNode_Son->m_x_str = CurNode_Son_str;
				/**********************将父节点信息和使能变迁放入子节点中**********************************/
				CurNode_Son->father_Node.push_back(list);
				//CurNode_Son->father_Node.push_back(CurNode);
				CurNode_Son->tran = t;
				/**********************计算子节点的h、f值****************************************************************/
				vector<int>max_value;
				for (int i = 0;i < CurNode_Son->m.size();i++)
				{
					if (CurNode_Son->m[i] > 0)
					{
						max_value.push_back(CurNode_Son->m[i] * atoi(place_info[i][1].c_str()));
						/*if (place_info[i][2] == "initial")
							CurNode_Son->distance = CurNode_Son->distance + CurNode_Son->m[i] * atoi(place_info[i][1].c_str());
						if (place_info[i][2] == "operator")
							CurNode_Son->distance = CurNode_Son->distance + CurNode_Son->m[i] * atoi(place_info[i][1].c_str());
						if (place_info[i][2] == "buff")
							CurNode_Son->distance = CurNode_Son->distance + CurNode_Son->m[i] * atoi(place_info[i][1].c_str());
						if (place_info[i][2] == "cleaning")
							CurNode_Son->distance = CurNode_Son->distance + delays[i];*/
					}
				}
				int max_h = *max_element(max_value.begin(), max_value.end());
				CurNode_Son->h = max_h;
				CurNode_Son->f = CurNode_Son->g + CurNode_Son->h;
				best_f_list.push(CurNode_Son);
			}
			/*****************************新旧结点判断************************************************************/
			while (!best_f_list.empty())
			{		
				/*shared_ptr<State> new_node(new State);
				new_node->m = best_f_list.top()->m;
				new_node->waitting_time = best_f_list.top()->waitting_time;
				new_node->g = best_f_list.top()->g;
				new_node->h = best_f_list.top()->h;
				new_node->f = best_f_list.top()->f;
				new_node->father_Node = best_f_list.top()->father_Node;
				new_node->tran = best_f_list.top()->tran;
				CurNode->childern_Node.push_back(new_node);	
				Open_list_temp.push_back(new_node);*/
				//auto _temp_visited_node = visited_nodes.find(best_f_list.top()->m_x_str)->second;
				if (visited_nodes.find(best_f_list.top()->m_x_str) == visited_nodes.end())
				{
					std::list<shared_ptr<State>>new_node_member;
					shared_ptr<State> new_node(new State);
					new_node->m = best_f_list.top()->m;
					new_node->waitting_time = best_f_list.top()->waitting_time;
					new_node->g = best_f_list.top()->g;
					new_node->h = best_f_list.top()->h;
					new_node->f = best_f_list.top()->f;
					new_node->father_Node = best_f_list.top()->father_Node;
					new_node->tran = best_f_list.top()->tran;
					CurNode->childern_Node.push_back(new_node);	
					Open_list_temp.push_back(new_node);
					new_node_member.push_back(new_node);
					visited_nodes.emplace(make_pair(best_f_list.top()->m_x_str, new_node_member));
				}
				else 
				{
					auto _temp_visited_node = visited_nodes.find(best_f_list.top()->m_x_str)->second;
					for (auto temp : _temp_visited_node)
					{
						if (!compare_G(temp, best_f_list.top()))
						{
							shared_ptr<State> new_node(new State);
							new_node->m = best_f_list.top()->m;
							new_node->waitting_time = best_f_list.top()->waitting_time;
							new_node->g = best_f_list.top()->g;
							new_node->f = best_f_list.top()->f;
							new_node->father_Node = best_f_list.top()->father_Node;
							new_node->tran = best_f_list.top()->tran;
							CurNode->childern_Node.push_back(new_node);
							if (compare_G(best_f_list.top(), temp))
							{
								temp->discard = true;						
								Open_list_temp.push_back(new_node);
								visited_nodes[best_f_list.top()->m_x_str].remove(temp);
								visited_nodes[best_f_list.top()->m_x_str].push_back(new_node);
							}	
							else
							{
								Open_list_temp.push_back(new_node);
							}
						}
					}
				}
				best_f_list.pop();
			}
			open_list_size--;
			if (open_list_size == 0)
			{				
				open_list_size = Open_list_temp.size();	
				step--;
				for (auto _open_temp : Open_list_temp)
				{
					/**************若目标在step!=0的出现则存入**********************/
					if (_open_temp->m[goal_place[0]] == goal_marking[0] && step != 0)
					{
						best_Son_node.push(_open_temp);
						//best_Son_node.push_back(_open_temp);
					}
				}				
				if (step == 0)
				{
					for (auto _temp : Open_list_temp)
					{
						best_Son_node.push(_temp);
						//best_Son_node.push_back(_temp);
					}
					continue;
				}
			}			
		}
		end = clock();
		cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
		/*while (best_Son_node.top()->discard == true)
		{
			best_Son_node.pop();
		}*/
		shared_ptr<State> best_Node = best_Son_node.top();
		//shared_ptr<State> best_Node = best_Son_node[0];
		while (best_Node->m != root->m)
		{
			sort(best_Node->father_Node.begin(), best_Node->father_Node.end(), compare);
			if (best_Node->father_Node[0]->m == root->m)
			{
				root = best_Node;
				continue;
			}
			best_Node = best_Node->father_Node[0];
		}
		Open_list.push(best_Node);
		//seek_childern_Son(Open_list, best_Node);
		//step_num = 5;//根据步长长度减一
		
		//best_Son_node.pop();
		//while (!best_Son_node.empty())
		//{
		//	auto temp = best_Son_node.top();
		//	temp->discard = true;
		//	//Open_list.push(temp);
		//	while (temp->m != best_Node->m)
		//	{
		//		sort(temp->father_Node.begin(), temp->father_Node.end(), compare);
		//		temp->father_Node[0]->discard = true;
		//		temp = temp->father_Node[0];
		//		//Open_list.push(temp);
		//	}
		//	best_Son_node.pop();
		//}
		//vector<shared_ptr<State>> _Open_list_temp;
		//auto _temp = Open_list;
		//while (!_temp.empty())
		//{
			//_Open_list_temp.push_back(_temp.top());
			//_temp.pop();
		//}
		//for (auto temp : _Open_list_temp)
		//{
			//if (temp->m != best_Node->m)
			//	temp->discard = true;
			//else
			//{
			//	temp->discard = false;
			//}
		//}
		//auto _temp_best_Node = best_Node;
		//while (!_temp_best_Node->childern_Node.empty())
		//{
			//sort(_temp_best_Node->childern_Node.begin(), _temp_best_Node->childern_Node.end(), compare);
			//_temp_best_Node->discard = false;
			//_temp_best_Node->childern_Node[0]->discard = false;
			//for (int i=1;i< _temp_best_Node->childern_Node.size();i++)
			//	_temp_best_Node->childern_Node[i]->discard=true;
			//_temp_best_Node = _temp_best_Node->childern_Node[0];
			/*best_Node->childern_Node.top()->discard = false;
			auto temp_best_Node = best_Node->childern_Node.top();
			best_Node->childern_Node.pop();
			while (!best_Node->childern_Node.empty())
			{
				best_Node->childern_Node.top()->discard = true;
				best_Node->childern_Node.pop();
			}
			best_Node = temp_best_Node;*/
		//}
		//Open_list.push(best_Node);
		//cout << "Open_list = " << Open_list.size() << endl;
	}
	
	

	
}



void Petri::Product_molde_new(vector<int> M, vector<int> m_Goal, vector<vector<int>> Pre, vector<vector<int>> Post, vector<int>delays, vector<int>goal_place, vector<int>goal_marking,
	vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info)
{
	/******************************记录时间、次数****************************************************/
	clock_t total_start, total_end;
	total_start = clock();
	int time_count = 0;
	//cin >> step;
	shared_ptr<State> inital_temp(new State);
	/*提取初始状态*/
	for (int i = 0;i < M.size();i++)
		inital_temp->m.push_back(M[i]);
	for (int i = 0;i < delays.size();i++)
		inital_temp->waitting_time.push_back(0);
	inital_temp->g = 0;
	inital_temp->f = 0;
	for (int i = 0;i < inital_temp->m.size();i++)
	{
		if (inital_temp->m[i] > 0)
		{
			inital_temp->distance = inital_temp->distance + inital_temp->m[i] * atoi(place_info[i][1].c_str());
		}

	}
	inital_temp->h = inital_temp->distance;
	inital_temp->f = inital_temp->g + inital_temp->h;
	Open_list.emplace(inital_temp);
	int count = 0;
	int total_time = 0;
	auto root = inital_temp;
	int step_num = 0;
	int num = 0;
	unordered_map<string, list<shared_ptr<State>>>visited_nodes;
	while (!Open_list.empty())
	{
		priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>best_Son_node;
		//vector<shared_ptr<State>>best_Son_node;

		vector<shared_ptr<State>>Open_list_temp;
		int step = 10 - step_num;//设置步长		
		num++;
		/*************************将open表中的members存入临时容器Open_list_temp*********************************************/
		while (!Open_list.empty())
		{
			/***************************************************/
			vector<string>root_m_info;//初始标识信息
			for (int i = 0;i < Open_list.top()->m.size();i++)
			{
				if (Open_list.top()->m[i] > 0)
					root_m_info.push_back(to_string(i).append("/").append(to_string(Open_list.top()->m[i])).append("/"));
			}
			string root_str;//定义个字符串将初始标识的库所id和token个数进行拼接
			for (int i = 0;i < root_m_info.size();i++)
			{
				root_str.append(root_m_info[i]);
			}
			/************设置一个临时open_list按顺序遍历**********/
			Open_list_temp.push_back(Open_list.top());
			std::list<shared_ptr<State>> member_list;
			member_list.push_back(Open_list.top());
			visited_nodes.emplace(make_pair(root_str, member_list));
			if (Open_list.top()->m[goal_place[0]] == goal_marking[0])
			{
				/************************************计算时间************************************************/
				total_end = clock();
				cout << "total_time = " << double(total_end - total_start) / CLOCKS_PER_SEC << "s" << endl;
				cout << "num = " << num << endl;
				cout << "average_time = " << double(total_end - total_start) / CLOCKS_PER_SEC / num << "s" << endl;
				cout << "f = " << Open_list.top()->f << " ";
				cout << "g = " << Open_list.top()->g << " ";
				cout << "h =" << Open_list.top()->h << " ";
				cout << endl;
				auto _root = Open_list.top();
				while (!_root->father_Node.empty())
				{
					int num = _root->tran;
					cout << "t" << num + 1 << " ";
					_root = _root->father_Node[0];
				}
				cout << endl;
			}
			Open_list.pop();
		}
		int open_list_size = Open_list_temp.size();
		clock_t start, end;
		start = clock();
		time_count++;
		while (step != 0 && !Open_list_temp.empty())
		{
			vector<int>enable_tran;
			priority_queue<shared_ptr<expand_State>, vector<shared_ptr<expand_State>>, f_cmp> best_f_list;
			//vector<shared_ptr<State>>best_f_list;
			shared_ptr<State>CurNode(new State);
			if (Open_list_temp[0]->discard == true)
			{
				Open_list_temp.erase(Open_list_temp.begin());
				continue;
			}
			auto list = Open_list_temp[0];
			CurNode = list;
			if (CurNode->m[goal_place[0]] == goal_marking[0])
			{
				/************************************计算时间************************************************/
				total_end = clock();
				cout << "total_time = " << double(total_end - total_start) / CLOCKS_PER_SEC << "s" << endl;
				cout << "num = " << num << endl;
				cout << "average_time = " << double(total_end - total_start) / CLOCKS_PER_SEC / num << "s" << endl;
				cout << "f = " << CurNode->f << " ";
				cout << "g = " << CurNode->g << " ";
				cout << "h =" << CurNode->h << " ";
				cout << endl;
				auto _root = CurNode;
				while (!_root->father_Node.empty())
				{
					int num = _root->tran;
					cout << "t" << num + 1 << " ";
					_root = _root->father_Node[0];
				}
				cout << endl;
			}
			/*************************************************************************/
			vector<string>CurNode_m_info;//已扩展节点的标识信息
			for (int i = 0;i < CurNode->m.size();i++)
			{
				if (CurNode->m[i] > 0)
					CurNode_m_info.push_back(to_string(i).append("/").append(to_string(CurNode->m[i])).append("/"));
			}
			string CurNode_str; //定义个字符串将已扩展节点标识的库所id和token个数进行拼接
			for (int i = 0;i < CurNode_m_info.size();i++)
			{
				CurNode_str.append(CurNode_m_info[i]);
			}
			/*************************************************************************/
			Open_list_temp.erase(Open_list_temp.begin());
			/***************************寻找使能变迁***********************************/
			int count = 0;
			for (int i = 0;i < Pre[0].size();i++)
			{
				count = 0;
				for (int j = 0;j < Pre.size();j++)
					if (CurNode->m[j] >= Pre[j][i])
						count++;
				if (count >= Pre.size())
				{
					count = 0;
					enable_tran.push_back(i);
				}
			}
			/********************使能变迁激发******************************************/
			for (auto t : enable_tran)
			{
				/********************计算lamba*****************************/
				shared_ptr<expand_State>CurNode_Son(new expand_State);
				vector<int> lamba;
				vector<int>unempty_place;
				for (int i = 0;i < transpose_Pre[t].size();i++)
				{
					if (transpose_Pre[t][i] > 0)
						unempty_place.push_back(i);
				}
				for (int i = 0;i < unempty_place.size();i++)
				{
					lamba.push_back(delays[unempty_place[i]] - CurNode->waitting_time[unempty_place[i]]);

				}
				int max_lamba = *max_element(lamba.begin(), lamba.end());
				if (max_lamba < 0)
					max_lamba = 0;
				/***********************计算m、已等待时间、g**********************************/
				for (int p = 0;p < Pre.size();p++)
				{
					CurNode_Son->m.push_back(CurNode->m[p] + Post[p][t] - Pre[p][t]);
					CurNode_Son->waitting_time.push_back(CurNode->waitting_time[p] - CurNode->waitting_time[p] * Pre[p][t] + max_lamba * (CurNode_Son->m[p] - Post[p][t]));
					if (CurNode_Son->waitting_time[p] < 0)
						CurNode_Son->waitting_time[p] = 0;
					if (delays[p] == 0)
						CurNode_Son->waitting_time[p] = 0;
				}
				CurNode_Son->g = CurNode->g + max_lamba;
				/***********************拼接子节点信息**************************************************/
				vector<string>CurNode_Son_m_info;//待扩展节点的标识信息
				for (int i = 0;i < CurNode_Son->m.size();i++)
				{
					if (CurNode_Son->m[i] > 0)
						CurNode_Son_m_info.push_back(to_string(i).append("/").append(to_string(CurNode_Son->m[i])).append("/"));
				}
				string CurNode_Son_str;//待扩展节点含有token的库所以及个数进行拼接
				for (int i = 0;i < CurNode_Son_m_info.size();i++)
				{
					CurNode_Son_str.append(CurNode_Son_m_info[i]);
				}
				CurNode_Son->m_x_str = CurNode_Son_str;
				/**********************将父节点信息和使能变迁放入子节点中**********************************/
				CurNode_Son->father_Node.push_back(CurNode);
				CurNode_Son->tran = t;
				/**********************计算子节点的h、f值****************************************************************/
				vector<int>max_value;
				for (int i = 0;i < CurNode_Son->m.size();i++)
				{
					if (CurNode_Son->m[i] > 0)
					{
						max_value.push_back(CurNode_Son->m[i] * atoi(place_info[i][1].c_str()));
					}
				}
				int max_h = *max_element(max_value.begin(), max_value.end());
				CurNode_Son->h = max_h;
				CurNode_Son->f = CurNode_Son->g + CurNode_Son->h;
				best_f_list.push(CurNode_Son);
			}
			std::list<shared_ptr<State>>Cur_node_member;
			Cur_node_member.push_back(CurNode);
			visited_nodes.emplace(make_pair(CurNode_str, Cur_node_member));
			/*****************************新旧结点判断************************************************************/
			while (!best_f_list.empty())
			{
				/*shared_ptr<State> new_node(new State);
				new_node->m = best_f_list.top()->m;
				new_node->waitting_time = best_f_list.top()->waitting_time;
				new_node->g = best_f_list.top()->g;
				new_node->h = best_f_list.top()->h;
				new_node->f = best_f_list.top()->f;
				new_node->father_Node = best_f_list.top()->father_Node;
				new_node->tran = best_f_list.top()->tran;
				CurNode->childern_Node.push_back(new_node);
				Open_list_temp.push_back(new_node);*/
				//auto _temp_visited_node = visited_nodes.find(best_f_list.top()->m_x_str)->second;
				if (visited_nodes.find(best_f_list.top()->m_x_str) == visited_nodes.end())
				{
					std::list<shared_ptr<State>>new_node_member;
					shared_ptr<State> new_node(new State);
					new_node->m = best_f_list.top()->m;
					new_node->waitting_time = best_f_list.top()->waitting_time;
					new_node->g = best_f_list.top()->g;
					new_node->h = best_f_list.top()->h;
					new_node->f = best_f_list.top()->f;
					new_node->father_Node = best_f_list.top()->father_Node;
					new_node->tran = best_f_list.top()->tran;
					CurNode->childern_Node.push_back(new_node);	
					Open_list_temp.push_back(new_node);
					new_node_member.push_back(new_node);
					visited_nodes.emplace(make_pair(best_f_list.top()->m_x_str, new_node_member));
				}
				else 
				{
					auto _temp_visited_node = visited_nodes.find(best_f_list.top()->m_x_str)->second;
					for (auto temp : _temp_visited_node)
					{
						if (!compare_G(temp, best_f_list.top()))
						{
							shared_ptr<State> new_node(new State);
							new_node->m = best_f_list.top()->m;
							new_node->waitting_time = best_f_list.top()->waitting_time;
							new_node->g = best_f_list.top()->g;
							new_node->f = best_f_list.top()->f;
							new_node->father_Node = best_f_list.top()->father_Node;
							new_node->tran = best_f_list.top()->tran;
							CurNode->childern_Node.push_back(new_node);
							if (compare_G(best_f_list.top(), temp))
							{
								temp->discard = true;						
								Open_list_temp.push_back(new_node);
								visited_nodes[best_f_list.top()->m_x_str].remove(temp);
								visited_nodes[best_f_list.top()->m_x_str].push_back(new_node);
							}	
							else
							{
								Open_list_temp.push_back(new_node);
							}
						}
					}
				}
				best_f_list.pop();
			}
			open_list_size--;
			if (open_list_size == 0)
			{
				open_list_size = Open_list_temp.size();
				step--;
				if (step == 0)
				{
					for (auto _temp : Open_list_temp)
					{
						best_Son_node.push(_temp);
						//best_Son_node.push_back(_temp);
					}
					continue;
				}
				
			}
		}
		end = clock();
		cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
		while (best_Son_node.top()->discard == true)
		{
			best_Son_node.pop();
		}
		shared_ptr<State> best_Node = best_Son_node.top();
		//vector<shared_ptr<State>>_best_Node;
		//while (best_Node->f == best_Son_node.top()->f&&!best_Son_node.empty())
		//{
		//	_best_Node.push_back(best_Son_node.top());
		//	best_Son_node.pop();
		//	
		//}
		//if (!_best_Node.empty())
		//{
		//	random_device rd;
		//	mt19937 gen(rd());
		//	uniform_int_distribution<size_t> dis(0, _best_Node.size() - 1);
		//	size_t randomIndex = dis(gen);
		//	best_Node = _best_Node[randomIndex];
		//	//vector<std::shared_ptr<State>> shuffledBestNode = _best_Node;
		//	//shuffle(shuffledBestNode.begin(), shuffledBestNode.end(), gen);
		//	//best_Node = shuffledBestNode[0];
		//	//uniform_int_distribution<unsigned> u(0, _best_Node.size() - 1);
		//	//default_random_engine e;
		//	//best_Node = _best_Node[u(e)];
		//}	
		//shared_ptr<State> best_Node = best_Son_node[0];
		while (best_Node->m != root->m) 
		{
			sort(best_Node->father_Node.begin(), best_Node->father_Node.end(), compare);
			if (best_Node->father_Node[0]->m == root->m)
			{
				root = best_Node;
				break;
			}
			best_Node = best_Node->father_Node[0];
		}
		seek_childern_Son(Open_list, best_Node);
		step_num = 14;//根据步长长度减一
	}
}


bool compare(shared_ptr<State>data1, shared_ptr<State>data2)
{
	if (data1->m == data2->m)
	{
		return data1->f < data2->f;
	}
	else if (data1->m != data2->m)
	{
		return data1->f < data2->f;
	}

}
inline bool compare_G(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g)
{
	vector<int>unempty_openlist;
	int count = 0;
	int temp = 0;
	for (int i = 0; i < Open_list_g->m.size();i++)
	{
		if (Open_list_g->m[i] > 0)
		{
			unempty_openlist.push_back(i);
		}			
	}
	for (int i = 0;i < unempty_openlist.size();i++)
	{
		if ( curNode_Son_g->g - curNode_Son_g->waitting_time[unempty_openlist[i]] < Open_list_g->g - Open_list_g->waitting_time[unempty_openlist[i]])
		{
			return false;
			//count++;
		}
	}
	return true;
		
	
}
inline bool compare_list_and_CurNode(unordered_map<string,list<shared_ptr<State>>>Open_list, pair<string, shared_ptr<State>>curNode_Son)
{
	if (Open_list.find(curNode_Son.first) == Open_list.end())
	{
		return true;
	}
	return false;
}
inline void seek_childern_Son(priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>&open_list,shared_ptr<State>Son)
{

	for (int i = 0;i < Son->childern_Node.size();i++)
	{
		auto temp = Son->childern_Node[i];
		seek_childern_Son(open_list, temp);
	}
	if (Son->childern_Node.size() == 0)
	{
		open_list.push(Son);

	}

}
inline bool compare_V(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g)
{
	vector<int>unempty_openlist;
	int count = 0;
	int temp = 0;
	for (int i = 0; i < Open_list_g->m.size();i++)
	{
		if (Open_list_g->m[i] > 0)
		{
			unempty_openlist.push_back(i);
		}
	}
	for (int i = 0;i < unempty_openlist.size();i++)
	{
		if (Open_list_g->waitting_time[unempty_openlist[i]]  >= curNode_Son_g->waitting_time[unempty_openlist[i]])
		{
			count++;
		}
	}
	if (count == unempty_openlist.size())
		return true;
	return false;
}
inline bool compare_V_son(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g)
{
	vector<int>unempty_openlist;
	int count = 0;
	int temp = 0;
	for (int i = 0; i < Open_list_g->m.size();i++)
	{
		if (Open_list_g->m[i] > 0)
		{
			unempty_openlist.push_back(i);
		}
	}
	for (int i = 0;i < unempty_openlist.size();i++)
	{
		if (Open_list_g->waitting_time[unempty_openlist[i]] > curNode_Son_g->waitting_time[unempty_openlist[i]])
		{
			return true;
		}
	}
	return false;	
}

//inline priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>Product_molde(priority_queue<shared_ptr<State>, vector<shared_ptr<State>>, open_cmp>list,int step
//	,vector<int>goal_place, vector<int>goal_marking,vector<int>delays,vector<vector<int>> Pre, vector<vector<int>> Post, vector<vector<int>>transpose_Pre,
//	vector<vector<int>>transpose_Post, 
//	vector<vector<string>>place_info)
//{
//	vector<shared_ptr<State>>temp_Node;
//	while (!list.empty())
//	{
//		temp_Node.push_back(list.top());
//		list.pop();
//	}
//		
//	while (step != 0)
//	{
//		while (!temp_Node.empty())
//		{
//			vector<int>enable_tran;
//			shared_ptr<State>curNode(new State);
//			curNode = temp_Node[0];
//			vector<string>curNode_m_info;//已扩展节点的标识信息
//			for (int i = 0;i < curNode->m.size();i++)
//			{
//				if (curNode->m[i] > 0)
//					curNode_m_info.push_back(to_string(i).append("/").append(to_string(curNode->m[i])).append("/"));
//			}
//			string curNode_str; //定义个字符串将已扩展节点标识的库所id和token个数进行拼接
//			for (int i = 0;i < curNode_m_info.size();i++)
//			{
//				curNode_str.append(curNode_m_info[i]);
//			}
//			temp_Node.erase(temp_Node.begin());
//			int count = 0;
//			for (int i = 0;i < Pre[0].size();i++)
//			{
//				count = 0;
//				for (int j = 0;j < Pre.size();j++)
//					if (curNode->m[j] >= Pre[j][i])
//						count++;
//				if (count >= Pre.size())
//				{
//					count = 0;
//					enable_tran.push_back(i);
//				}
//			}
//			for (auto t : enable_tran)
//			{
//				shared_ptr<State>curNode_Son(new State);
//				vector<int> lamba;
//				vector<int>unempty_place;
//
//				for (int i = 0;i < transpose_Pre[t].size() - 3;i++)
//				{
//					if (transpose_Pre[t][i] > 0)
//						unempty_place.push_back(i);
//				}
//				for (int i = 0;i < unempty_place.size();i++)
//				{
//					lamba.push_back(delays[unempty_place[i]] - curNode->waitting_time[unempty_place[i]]);
//
//				}
//
//				int max_lamba = *max_element(lamba.begin(), lamba.end());
//				if (max_lamba < 0)
//					max_lamba = 0;
//
//				for (int p = 0;p < Pre.size();p++)
//				{
//					curNode_Son->m.push_back(curNode->m[p] + Post[p][t] - Pre[p][t]);
//					curNode_Son->waitting_time.push_back(curNode->waitting_time[p] - curNode->waitting_time[p] * Pre[p][t] + max_lamba * (curNode_Son->m[p] - Post[p][t]));
//					if (curNode_Son->waitting_time[p] < 0)
//						curNode_Son->waitting_time[p] = 0;
//					if (delays[p] == 0)
//						curNode_Son->waitting_time[p] = 0;
//				}
//				curNode_Son->g = curNode->g + max_lamba;
//				vector<string>curNode_Son_m_info;//待扩展节点的标识信息
//				for (int i = 0;i < curNode_Son->m.size();i++)
//				{
//					if (curNode_Son->m[i] > 0)
//						curNode_Son_m_info.push_back(to_string(i).append("/").append(to_string(curNode_Son->m[i])).append("/"));
//				}
//				string curNode_Son_str;//待扩展节点含有token的库所以及个数进行拼接
//				for (int i = 0;i < curNode_Son_m_info.size();i++)
//				{
//					curNode_Son_str.append(curNode_Son_m_info[i]);
//				}
//				curNode_Son->father_Node.push_back(curNode);
//				curNode_Son->tran = t;
//
//				/******************************************************************************************/
//				for (int i = 0;i < curNode_Son->m.size();i++)
//				{
//					if (curNode_Son->m[i] > 0)
//					{
//						if (place_info[i][2] == "initial")
//							curNode_Son->distance = curNode_Son->distance + curNode_Son->m[i] * atoi(place_info[i][1].c_str());
//						if (place_info[i][2] == "operator")
//							curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
//						if (place_info[i][2] == "buff")
//							curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
//						if (place_info[i][2] == "cleaning")
//							curNode_Son->distance = curNode_Son->distance + delays[i];
//					}
//				}
//				curNode_Son->h = curNode_Son->distance;
//				curNode_Son->f = curNode_Son->g + curNode_Son->distance;
//
//
//				if (curNode_Son->m[goal_place[0]] == goal_marking[0])
//				{
//					list.push(curNode_Son);
//					//Close_list[curNode_Son_str].push_back(curNode_Son);
//					//exit(1);
//					//cout << "f = " << curNode_Son->f << " ";
//					//cout << "g = " << curNode_Son->g << " ";
//					//cout << endl;
//					continue;
//				}
//				list.push(curNode_Son);
//			}
//		}
//		Product_molde(list, --step, goal_place, goal_marking, delays, Pre, Post, transpose_Pre, transpose_Post, place_info);
//		if (!list.empty())
//		{
//			for (auto father : list.top()->father_Node)
//			{
//				father->priorty = true;
//			}
//		}
//	}
//	
//	
//	
//	return list;
//}


