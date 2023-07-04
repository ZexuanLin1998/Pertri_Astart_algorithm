constexpr auto M0_Path = ".\\data\\M0.txt";
constexpr auto Pre_Path = ".\\data\\Pre.txt";
constexpr auto Post_Path = ".\\data\\Post.txt";
constexpr auto Goal_Path = ".\\data\\goal.txt";
constexpr auto Delays_Path = ".\\data\\delays.txt";
constexpr auto Goal_place = ".\\data\\GoalPlace.txt";
constexpr auto Goal_marking = ".\\data\\GoalMarking.txt";
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using std::vector;
using std::string;
vector<int> read_vector(const char* path) {
	int num;
	vector<int> result;
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "fail to file" << std::endl;
	}
	while (file >> num)
		result.push_back(num);
	file.close();
	return result;
}
inline vector<vector<int>> read_matrix(const char* path, int m)
{
	vector<vector<int>> matrix;
	std::ifstream file(path, std::ios::in);
	if (!file.is_open())
	{
		std::cout << "can not open file:" << path << std::endl;
		exit(1);
	}
	int num = 0;
	vector<int> ans;
	while (file >> num) {
		ans.push_back(num);
	}
	int n = ans.size() / m;
	matrix.resize(m, vector<int>(n, 0));
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = ans[i * n + j];
	file.close();
	return matrix;
}
/*矩阵转秩*/
inline vector<vector<int>> transposeInPlace(vector<vector<int>> m) {
	//矩阵转至
	vector<vector<int>> array;
	vector<int> temparay;
	for (int i = 0; i < m[0].size(); ++i)// m*n 维数组
	{
		for (int j = 0; j < m.size(); ++j) {
			temparay.push_back(m[j][i]);
		}
		array.push_back(temparay);
		temparay.erase(temparay.begin(), temparay.end());
	}
	return array;
}
