#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//������� ������ ������� ������ �������
unsigned find_median(std::vector<std::string> &src, std::string &templ)
{
	unsigned err_word = 0, min_err_word = -1, err_all = 0;
	for (unsigned i = 0; i < src.size(); ++i) // ���� �� ���� ��������
	{
		for (unsigned j = 0; j + templ.size() < src[i].size(); ++j) // ���� �������� ��������� �������� ������ ����� �������
		{
			for (unsigned k = 0; k < templ.size(); ++k) // ���� �������� ���������� ������ �������
				if (templ[k] != src[i][j + k]) err_word++; // ��� ������������ ���������� � ������� � � �������, ������������� ������� ������
			if (min_err_word > err_word) min_err_word = err_word; // ���������� ��������� ������������ � ������� �� ������ ������
			err_word = 0;
		}
		err_all += min_err_word; // ���� ��������� �� ������ ������
		min_err_word = -1;
	}
	return err_all;
}

// ������� ��������� ���� ��������� ��������
void fill_vec_tmpl(std::vector<std::string> &vec_tmpl, unsigned sample_length, std::string str = "")
{
	if (str.size() + 1 == sample_length) // ���� ������� ����������� ��������� ���������, ���������� � ������� 4 ��������
	{
		vec_tmpl.push_back(str + "A");
		vec_tmpl.push_back(str + "C");
		vec_tmpl.push_back(str + "G");
		vec_tmpl.push_back(str + "T");
	}
	else // ���� ���������� ��������� ��������, ��� ���������� ��������� ����������� ������� ����������
	{
		fill_vec_tmpl(vec_tmpl, sample_length, str + "A");
		fill_vec_tmpl(vec_tmpl, sample_length, str + "C");
		fill_vec_tmpl(vec_tmpl, sample_length, str + "G");
		fill_vec_tmpl(vec_tmpl, sample_length, str + "T");
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
	std::vector<std::string> src; // ������ ��� �������� ���� �������� �������
	unsigned sample_length;
	fsrc >> sample_length;// ����������� ����� �������
	if (sample_length == 0) // �������� �� ������� ������, �������� ������ �����.
	{
		std::ofstream result("output.txt");
		result.close();
		return 0;
	}
	std::string cur_line;
	while (fsrc.good()) // ������������ �� ����� ������ � ������������ � ������ �� ���������� ����� ����� (� ���� ��� ��� ���������� �����, ���������� ���)
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
	std::vector<std::string> vec_shab; // ������ ��� �������� ���� ��������� ��������
	fill_vec_tmpl(vec_shab, sample_length); // ��������� ���� ��������
	unsigned min_index = 0, min_median = find_median(src, vec_shab[0]); // ������������� ������ ������� ��� ����������� ���������
	for (unsigned i = 1; i < vec_shab.size(); ++i)  // ��� ������� ������� ��������� ������� � ������������ � �����������.
	{
		unsigned cur_median = find_median(src, vec_shab[i]);
		if (cur_median < min_median)
		{
			min_median = cur_median;
			min_index = i; // ���������� ������������ ������� ��� ������ ������������ �������
		}
	}
	std::ofstream result("output.txt");
	result << vec_shab[min_index] << std::endl;
	result.close();
	return 0;
}