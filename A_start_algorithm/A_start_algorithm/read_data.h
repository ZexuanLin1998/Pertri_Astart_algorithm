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
/*����ת��*/
inline vector<vector<int>> transposeInPlace(vector<vector<int>> m) {
	//����ת��
	vector<vector<int>> array;
	vector<int> temparay;
	for (int i = 0; i < m[0].size(); ++i)// m*n ά����
	{
		for (int j = 0; j < m.size(); ++j) {
			temparay.push_back(m[j][i]);
		}
		array.push_back(temparay);
		temparay.erase(temparay.begin(), temparay.end());
	}
	return array;
}

/*��ȡcsv�ļ�*/
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
	vector<string>words;//����һ���ַ�������
	string word;
	/*******************��ȡ����****************************/
	//��ȡ������
	getline(csv_data, line);

	istringstream sin;
	//���ж�ȡ����
	while (getline(csv_data,line))
	{
		//���vector���ַ�������ֻ�浱ǰ�е�����
		words.clear();
		sin.clear();

		sin.str(line);
		//���ַ�����sin�е��ַ��������ַ�������words�У��Զ���Ϊ�ָ���
		while (getline(sin, word, ','))
		{
			//std::cout << word << std::endl;
			words.push_back(word);//��ÿһ�����е��������push
		}

		//��������е�����
		//PrintCSVLine(words);
		arry.push_back(words);
	}
	csv_data.close();

	return arry;
}