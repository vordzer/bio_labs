#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//Функция поиска score для конкретных позиций s
unsigned score(std::vector<unsigned> &cur_s, std::vector<std::string> &src, unsigned sample_length)
{
	unsigned res = 0;
	for (unsigned i = 0; i < sample_length; ++i) //проверяется от стартовой позиции до длины шаблона
	{
		unsigned cur_nucl_a = 0, cur_nucl_g = 0, cur_nucl_c = 0, cur_nucl_t = 0;
		for (auto j = 0; j < src.size(); ++j)
		{
			// из вектора всех строк берётся строка с индексом src[j], в ней выбирается символ с индексом[cur_s[j] + i] и сравнивается с возможным значением нуклеотидов
			if (src[j][cur_s[j] + i] == 'A' || src[j][cur_s[j] + i] == 'a') cur_nucl_a++;
			if (src[j][cur_s[j] + i] == 'G' || src[j][cur_s[j] + i] == 'g') cur_nucl_g++;
			if (src[j][cur_s[j] + i] == 'C' || src[j][cur_s[j] + i] == 'c') cur_nucl_c++;
			if (src[j][cur_s[j] + i] == 'T' || src[j][cur_s[j] + i] == 't') cur_nucl_t++;
		}
		// Выбирается максимальное значение на данной позиции шаблона
		unsigned max_nucl = cur_nucl_a;
		if (cur_nucl_g > max_nucl) max_nucl = cur_nucl_g;
		if (cur_nucl_c > max_nucl) max_nucl = cur_nucl_c;
		if (cur_nucl_t > max_nucl) max_nucl = cur_nucl_t;
		res += max_nucl;
	}
	return res;
}

// Функция  для поиска лучшего мотива, позиции такого мотива записываются в best_s
void search_best_motif(std::vector<unsigned> &cur_s, std::vector<unsigned> &best_s, std::vector<std::string> &src, unsigned sample_length, unsigned &best_score, unsigned line)
{
	for (auto i = 0; i + sample_length <= src[line].size(); ++i) // Проход по всем возможным начальным значениям шаблона в данной строке
	{
		cur_s[line] = i;
		if (line + 1 < src.size()) // Проверка на последнюю строку
		{ // Если строка не последняя, то рекурсивно вызывается поиск мотива для строки ниже
			search_best_motif(cur_s, best_s, src, sample_length, best_score, line + 1);
		}
		else
		{ // Если строка последняя, то ищется её лучший мотив
			unsigned cur_score = score(cur_s, src, sample_length);
			if (cur_score >= best_score) // Достаточно использования ">" вместо ">=", но тестовые данные содержат два равнозначных массива. При смене типа сравнения коррекстность работы сохраняется, но вывод не совпадает с презентацией
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
	if (!fsrc.is_open()) //Проверка существования файла ввода
	{
		std::cout << "Input file not found\n";
		return 1;
	}
	std::vector<std::string> src;
	unsigned sample_length, count_line;
	fsrc >> sample_length >> count_line;// Вычитывание  длины шаблона и количества строк
	std::string cur_line;
	for (unsigned i = 0; i < count_line; ++i) // Вычитывается по одной строке и записывается в вектор
	{
		fsrc >> cur_line;
		src.push_back(cur_line);
		cur_line.clear();
	}
	fsrc.close();
	if (src[0].size() < sample_length) //Проверка длины
	{
		std::cout << "Source line too short\n";
		return 2;
	}
	std::vector<unsigned> best_s(count_line), cur_s(count_line); // Вычитывается векторы с текущими позициями S, и лучшей S
	unsigned best_score = 0;
	search_best_motif(cur_s, best_s, src, sample_length, best_score, 0);// 0 означает поиск с самого начала, это проще написания отдельного интерфейса
	std::ofstream result("output.txt");
	for (unsigned i = 0; i < src.size(); ++i) // Построчный вывод шаблона из лучших позиций
		result << src[i].substr(best_s[i], sample_length) << std::endl;
	result.close();
	return 0;
}