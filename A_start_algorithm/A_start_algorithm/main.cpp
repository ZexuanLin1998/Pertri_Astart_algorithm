#include<iostream>
#include<vector>
#include"Petri.h"
#include"read_data.h"
#include<string>

using namespace std;

int main()
{
	auto pn = new Petri();
	std::vector<int>M0 = read_vector(M0_Path);
	std::vector<int>delays = read_vector(Delays_Path);
	std::vector<int>m_goal = read_vector(Goal_Path);
	std::vector<vector<string>> place_info = read_csv(Place_info);
	std::vector<int>goal_place = read_vector(Goal_place);
	std::vector<int>goal_marking = read_vector(Goal_marking);
	std::vector<int>target_distance = read_vector(Target_distance);
	int place_num = M0.size();
	std::vector<vector<int>>Pre = read_matrix(Pre_Path, place_num);
	std::vector<vector<int>>Post = read_matrix(Post_Path, place_num);
	std::vector<vector<int>>transpose_Pre = transposeInPlace(Pre);
	std::vector<vector<int>>transpose_Post = transposeInPlace(Post);
	//pn->play(M0, m_goal, Pre, Post, delays, goal_place, goal_marking, transpose_Pre, transpose_Post, place_info);
	//pn->Product_molde(M0, m_goal, Pre, Post,delays, goal_place, goal_marking, transpose_Pre, transpose_Post, place_info);
	pn->Product_molde_new(M0, m_goal, Pre, Post, delays, goal_place, goal_marking, transpose_Pre, transpose_Post, place_info);
	return 0;
}