constexpr auto M0_Path = ".\\data\\M0.txt";
constexpr auto Pre_Path = ".\\data\\Pre.txt";
constexpr auto Post_Path = ".\\data\\Post.txt";
constexpr auto Goal_Path = ".\\data\\goal.txt";
constexpr auto Delays_Path = ".\\data\\delays.txt";
constexpr auto Goal_place = ".\\data\\GoalPlace.txt";
constexpr auto Goal_marking = ".\\data\\GoalMarking.txt";
constexpr auto Target_distance = ".\\data\\target_distance.txt";
constexpr auto Place_info = ".\\data\\place_info.csv";
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
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

/*读取csv文件*/
inline vector<vector<string>> read_csv(const char* path)
{
	std::ifstream csv_data(path, ios::in);
	
	if (!csv_data.is_open())
	{
		std::cout << "Error: opening file fail" << std::endl;
		exit(1);
	}
	
	string line;
	
	vector<vector<string>>arry;
	vector<string>words;//声明一个字符串向量
	string word;
	/*******************读取数据****************************/
	//读取标题行
	getline(csv_data, line);

	istringstream sin;
	//按行读取数据
	while (getline(csv_data,line))
	{
		//清空vector及字符串流，只存当前行的数据
		words.clear();
		sin.clear();

		sin.str(line);
		//将字符串流sin中的字符串读到字符串数组words中，以逗号为分隔符
		while (getline(sin, word, ','))
		{
			//std::cout << word << std::endl;
			words.push_back(word);//将每一个格中的数据逐个push
		}

		//输出此行中的内容
		//PrintCSVLine(words);
		arry.push_back(words);
	}
	csv_data.close();

	return arry;
}