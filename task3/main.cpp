#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//������� ������ score ��� ���������� ������� s
unsigned score(std::vector<unsigned> &cur_s, std::vector<std::string> &src, unsigned sample_length)
{
	unsigned res = 0;
	for (unsigned i = 0; i < sample_length; ++i) //����������� �� ��������� ������� �� ����� �������
	{
		unsigned cur_nucl_a = 0, cur_nucl_g = 0, cur_nucl_c = 0, cur_nucl_t = 0;
		for (auto j = 0; j < src.size(); ++j)
		{
			// �� ������� ���� ����� ������ ������ � �������� src[j], � ��� ���������� ������ � ��������[cur_s[j] + i] � ������������ � ��������� ��������� �����������
			if (src[j][cur_s[j] + i] == 'A' || src[j][cur_s[j] + i] == 'a') cur_nucl_a++;
			if (src[j][cur_s[j] + i] == 'G' || src[j][cur_s[j] + i] == 'g') cur_nucl_g++;
			if (src[j][cur_s[j] + i] == 'C' || src[j][cur_s[j] + i] == 'c') cur_nucl_c++;
			if (src[j][cur_s[j] + i] == 'T' || src[j][cur_s[j] + i] == 't') cur_nucl_t++;
		}
		// ���������� ������������ �������� �� ������ ������� �������
		unsigned max_nucl = cur_nucl_a;
		if (cur_nucl_g > max_nucl) max_nucl = cur_nucl_g;
		if (cur_nucl_c > max_nucl) max_nucl = cur_nucl_c;
		if (cur_nucl_t > max_nucl) max_nucl = cur_nucl_t;
		res += max_nucl;
	}
	return res;
}

// �������  ��� ������ ������� ������, ������� ������ ������ ������������ � best_s
void search_best_motif(std::vector<unsigned> &cur_s, std::vector<unsigned> &best_s, std::vector<std::string> &src, unsigned sample_length, unsigned &best_score, unsigned line)
{
	for (auto i = 0; i + sample_length <= src[line].size(); ++i) // ������ �� ���� ��������� ��������� ��������� ������� � ������ ������
	{
		cur_s[line] = i;
		if (line + 1 < src.size()) // �������� �� ��������� ������
		{ // ���� ������ �� ���������, �� ���������� ���������� ����� ������ ��� ������ ����
			search_best_motif(cur_s, best_s, src, sample_length, best_score, line + 1);
		}
		else
		{ // ���� ������ ���������, �� ������ � ������ �����
			unsigned cur_score = score(cur_s, src, sample_length);
			if (cur_score >= best_score) // ���������� ������������� ">" ������ ">=", �� �������� ������ �������� ��� ������������ �������. ��� ����� ���� ��������� ������������� ������ �����������, �� ����� �� ��������� � ������������
			{
				best_score = cur_score;
				best_s = cur_s;
			}
		}
	}
}

int main()
{
	std::ifstream fsrc("input.txt");
	if (!fsrc.is_open()) //�������� ������������� ����� �����
	{
		std::cout << "Input file not found\n";
		return 1;
	}
	std::vector<std::string> src;
	unsigned sample_length, count_line;
	fsrc >> sample_length >> count_line;// �����������  ����� ������� � ���������� �����
	std::string cur_line;
	for (unsigned i = 0; i < count_line; ++i) // ������������ �� ����� ������ � ������������ � ������
	{
		fsrc >> cur_line;
		src.push_back(cur_line);
		cur_line.clear();
	}
	fsrc.close();
	if (src[0].size() < sample_length) //�������� �����
	{
		std::cout << "Source line too short\n";
		return 2;
	}
	std::vector<unsigned> best_s(count_line), cur_s(count_line); // ������������ ������� � �������� ��������� S, � ������ S
	unsigned best_score = 0;
	search_best_motif(cur_s, best_s, src, sample_length, best_score, 0);// 0 �������� ����� � ������ ������, ��� ����� ��������� ���������� ����������
	std::ofstream result("output.txt");
	for (unsigned i = 0; i < src.size(); ++i) // ���������� ����� ������� �� ������ �������
		result << src[i].substr(best_s[i], sample_length) << std::endl;
	result.close();
	return 0;
}