#include "Petri.h"
#include<memory>
#include<algorithm>
#include <iostream>
bool compare_G(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g);
inline bool compare_list_and_CurNode(unordered_map<string, list<shared_ptr<State>>>Open_list, pair<string, shared_ptr<State>>curNode_Son);
void Petri::play(vector<int> M, vector<int> m_Goal, vector<vector<int>> Pre, vector<vector<int>> Post,vector<int>delays,vector<int>goal_place,vector<int>goal_marking,
	vector<vector<int>>transpose_Pre, vector<vector<int>>transpose_Post, vector<vector<string>>place_info)
{
	shared_ptr<State> inital_temp(new State);
	for (int i = 0;i < M.size();i++)
		inital_temp->m.push_back(M[i]);
	for (int i = 0;i < delays.size();i++)
		inital_temp->waitting_time.push_back(0);
	inital_temp->g = 0;
	/*修改*/
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
	//inital_temp->name = init_str;
	inital_temp->discard = false;
	Open_list.emplace(inital_temp);
	int count = 0;


	while (!Open_list.empty())
	{
		vector<int>enable_tran;
		shared_ptr<State> curNode(new State);
		pair<string, shared_ptr<State>>curNode_temp;//定义一个已扩展节点的缓存容器
		/**************************************************************************/
		auto list = Open_list.top();
		curNode->m = list->m;
		curNode->waitting_time = list->waitting_time;
		curNode->g = list->g;
		curNode->discard = list->discard;
		curNode->distance = list->distance;
		curNode->f = curNode->g + curNode->distance;
		if (Open_list.top()->discard == true )
		{
			Open_list.pop();
			continue;
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
		curNode_temp.first = curNode_str;
		curNode_temp.second = curNode;
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
			pair<string, shared_ptr<State>>curNode_Son_temp;//定义一个待扩展节点的缓存容器
			vector<int> lamba;
			vector<int>unempty_place;
			
			for (int i = 0;i < transpose_Pre[t].size()-3;i++)
			{
				if (transpose_Pre[t][i] > 0)
					unempty_place.push_back(i);
			}
			for (int i=0;i< unempty_place.size();i++)
			{
				lamba.push_back(delays[unempty_place[i]] - curNode->waitting_time[unempty_place[i]]);
				
			}
			/*for (int i = 0;i < lamba.size();i++)
			{
				cout << "lamba[" << i << "] = " << lamba[i] << " ";
			}
			cout << endl;*/
			int max_lamba = *max_element(lamba.begin(),lamba.end());
			if (max_lamba < 0)
				max_lamba = 0;
			//cout << " max_lamba : "<<max_lamba << " ";
			//cout << endl;

			for (int p = 0;p < Pre.size();p++)
			{
				curNode_Son->m.push_back(curNode->m[p] + Post[p][t] - Pre[p][t]);
				curNode_Son->waitting_time.push_back(curNode->waitting_time[p] - curNode->waitting_time[p] * Pre[p][t] + max_lamba * (curNode_Son->m[p] - Post[p][t]));	
				if (curNode_Son->waitting_time[p] < 0)
					curNode_Son->waitting_time[p] = 0;
				if (delays[p] == 0)
					curNode_Son->waitting_time[p] = 0;
			}
			/*for (int i= 0;i < unempty_place.size();i++)
			{
				curNode_Son->waitting_time[unempty_place[i]]=curNode->waitting_time[unempty_place[i]] -
					curNode->waitting_time[unempty_place[i]] * Pre[unempty_place[i]][t] + max_lamba * (curNode_Son->m[unempty_place[i]] - Post[unempty_place[i]][t]);
			}*/
			curNode_Son->g = curNode->g + max_lamba;
			//cout << curNode_Son->g << endl;
			vector<string>curNode_Son_m_info;//待扩展节点的标识信息
			for (int i = 0;i < curNode_Son->m.size();i++)
			{
				if (curNode_Son->m[i] > 0)
					curNode_Son_m_info.push_back(to_string(i).append("/").append(to_string(curNode_Son->m[i])).append("/"));
			}
			string curNode_Son_str;//待扩展节点含有token的库所以及个数进行拼接
			for (int i = 0;i < curNode_Son_m_info.size();i++)
			{
				curNode_Son_str.append(curNode_Son_m_info[i]);
			}
			curNode_Son->father_Node.push_back(curNode);
			curNode_Son_temp.first = curNode_Son_str;
			curNode_Son_temp.second = curNode_Son;
			/******************************************************************************************/
			for (int i = 0;i < curNode_Son->m.size();i++)
			{
				if (curNode_Son->m[i] > 0)
				{
					if (place_info[i][2] == "initial")
						curNode_Son->distance = curNode_Son->distance + curNode_Son->m[i] * atoi(place_info[i][1].c_str());
					if (place_info[i][2] == "operator")
						curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
					if (place_info[i][2] == "buff")
						curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
					if (place_info[i][2] == "cleaning")
						curNode_Son->distance = curNode_Son->distance + delays[i];
				}
			}
			curNode_Son->f = curNode_Son->g + curNode_Son->distance;
			//cout << "g = " << curNode_Son->g << endl;
			/*if (curNode_temp.first == "0/1/1/1/7/1/11/2/15/3/18/1/24/3/27/1/28/1/29/1/30/1/31/1/32/1/34/1/")
			{
				int c = 0;
			}*/
			/*将已扩展节点存入close_list中*/
		    std::list<shared_ptr<State>> member_list;
			member_list.push_back(curNode_temp.second);
			Close_list.emplace(make_pair(curNode_str, member_list));
			
			if (curNode_Son->m[goal_place[0]] == goal_marking[0])
			{
				
				Close_list[curNode_Son_temp.first].push_back(curNode_Son_temp.second);
				//exit(1);
				cout << "f = " << curNode_Son_temp.second->f << " ";
				cout << "g = " << curNode_Son_temp.second->g << " ";
				cout << endl;
			}
			/*for (int i = 0;i < curNode_Son->m.size();i++)
			{
				if (curNode_Son->m[i] > 0)
				{
					if (place_info[i][2] == "initial")
						curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
					if (place_info[i][2] == "operator")
						curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
					if (place_info[i][2] == "buff")
						curNode_Son->distance = curNode_Son->distance + atoi(place_info[i][1].c_str());
					if (place_info[i][2] == "cleaning")
						curNode_Son->distance = curNode_Son->distance + delays[i];
				}
			}*/

			if (Close_list.find(curNode_Son_temp.first) == Close_list.end())
			{	
				std::list<shared_ptr<State>>curNode_member;
				Open_list.emplace(curNode_Son_temp.second);
				curNode_member.push_back(curNode_Son_temp.second);
				Close_list.emplace(make_pair(curNode_Son_temp.first,curNode_member));
			}
			else if(Close_list.find(curNode_Son_temp.first) != Close_list.end())
			{
				auto c_list = Close_list[curNode_Son_temp.first];
				for (auto it : c_list)
				{
					if (compare_G(it, curNode_Son_temp.second))
					{
						if (compare_G(curNode_Son_temp.second, it))
						{
							it->discard = true;
							//Close_list.erase(Close_list.find(curNode_Son_temp.first));
							Close_list[curNode_Son_temp.first].push_back(curNode_Son_temp.second);
							curNode_Son_temp.second->discard = false;
							Open_list.emplace(curNode_Son_temp.second);
						}
					}
				}

			}
			
		}	
	}
	cout << " 已扩展结点" <<Close_list.size() << endl;

}



inline bool compare_G(shared_ptr<State>Open_list_g, shared_ptr<State>curNode_Son_g)
{
	vector<int>unempty_openlist;
	int count = 0;
	for (int i = 0; i < Open_list_g->m.size();i++)
	{
		if (Open_list_g->m[i] > 0)
		{
			unempty_openlist.push_back(i);
		}			
	}
	for (int i = 0;i < unempty_openlist.size();i++)
	{
		if (Open_list_g->waitting_time[unempty_openlist[i]] + (curNode_Son_g->g - Open_list_g->g) >=  curNode_Son_g->waitting_time[unempty_openlist[i]])
		{
			count++;
		}
	}
	if (count == unempty_openlist.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}
inline bool compare_list_and_CurNode(unordered_map<string,list<shared_ptr<State>>>Open_list, pair<string, shared_ptr<State>>curNode_Son)
{
	//while (!Open_list.empty())
	//{
	//	if (Open_list.top()->name == curNode_Son.first)
	//		return false;
	//	Open_list.pop();
	//	return true;
	//}
	if (Open_list.find(curNode_Son.first) == Open_list.end())
	{
		return true;
	}
	return false;
}

