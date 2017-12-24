#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//‘ункци€ поиска score дл€ конкретных позиций s
unsigned score(std::vector<unsigned> &cur_s, std::vector<std::string> &src, unsigned sample_length, unsigned row_count, unsigned &best_score)
{
	unsigned res = 0;
	for (unsigned i = 0; i < sample_length; ++i) //провер€етс€ от стартовой позиции до длины шаблона
	{
		unsigned cur_nucl_a = 0, cur_nucl_g = 0, cur_nucl_c = 0, cur_nucl_t = 0;
		for (auto j = 0; j < row_count; ++j)
		{
			// из вектора всех строк берЄтс€ строка с индексом src[j], в ней выбираетс€ символ с индексом[cur_s[j] + i] и сравниваетс€ с возможным значением нуклеотидов
			if (src[j][cur_s[j] + i] == 'A' || src[j][cur_s[j] + i] == 'a') cur_nucl_a++;
			if (src[j][cur_s[j] + i] == 'G' || src[j][cur_s[j] + i] == 'g') cur_nucl_g++;
			if (src[j][cur_s[j] + i] == 'C' || src[j][cur_s[j] + i] == 'c') cur_nucl_c++;
			if (src[j][cur_s[j] + i] == 'T' || src[j][cur_s[j] + i] == 't') cur_nucl_t++;
		}
		// ¬ыбираетс€ максимальное значение на данной позиции шаблона
		unsigned max_nucl = cur_nucl_a;
		if (cur_nucl_g > max_nucl) max_nucl = cur_nucl_g;
		if (cur_nucl_c > max_nucl) max_nucl = cur_nucl_c;
		if (cur_nucl_t > max_nucl) max_nucl = cur_nucl_t;
		res += max_nucl;
		if (res + (sample_length - i) * row_count + sample_length * (src.size() - row_count) < best_score) // останавливаем линейный поиск, если уже не будет лучшего результата
			return 0;
	}
	return res;
}

// ‘ункци€  дл€ поиска лучшего мотива, позиции такого мотива записываютс€ в best_s
void search_best_motif(std::vector<unsigned> &cur_s, std::vector<unsigned> &best_s, std::vector<std::string> &src, unsigned sample_length, unsigned &best_score, unsigned line)
{
	for (auto i = 0; i + sample_length <= src[line].size(); ++i) // ѕроход по всем возможным начальным значени€м шаблона в данной строке
	{
		cur_s[line] = i;
		if (line + 1 < src.size()) // ѕроверка на последнюю строку
		{ // ≈сли строка не последн€€, то рекурсивно вызываетс€ поиск мотива дл€ строки ниже
			if (score(cur_s, src, sample_length, line, best_score) + sample_length * (src.size() - line) >= best_score) // ѕродолжаем искать, если возможнен новый best_score
				search_best_motif(cur_s, best_s, src, sample_length, best_score, line + 1);
		}
		else
		{ // ≈сли строка последн€€, то ищетс€ еЄ лучший мотив
			unsigned cur_score = score(cur_s, src, sample_length, src.size(), best_score);
			if (cur_score >= best_score) // ƒостаточно использовани€ ">" вместо ">=", но тестовые данные содержат два равнозначных массива. ѕри смене типа сравнени€ коррекстность работы сохран€етс€, но вывод не совпадает с презентацией
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
	if (!fsrc.is_open()) //ѕроверка существовани€ войда ввода
	{
		std::cout << "Input file not found\n";
		return 1;
	}
	std::vector<std::string> src;
	unsigned sample_length, count_line;
	fsrc >> sample_length >> count_line;// ¬ычитывание  длины шаблона и количества строк
	std::string cur_line;
	for (unsigned i = 0; i < count_line; ++i) // ¬ычитываетс€ по одной строке и записываетс€ в вектор
	{
		fsrc >> cur_line;
		src.push_back(cur_line);
		cur_line.clear();
	}
	fsrc.close();
	if (src[0].size() < sample_length) //ѕроверка длины
	{
		std::cout << "Source line too short\n";
		return 2;
	}
	std::vector<unsigned> best_s(count_line), cur_s(count_line); // ¬ычитываетс€ векторы с текущими позици€ми S, и лучшей S
	unsigned best_score = 0;
	search_best_motif(cur_s, best_s, src, sample_length, best_score, 0);// 0 означает поиск с самого начала, это проще написани€ отдельного интерфейса
	std::ofstream result("output.txt");
	for (unsigned i = 0; i < src.size(); ++i) // ѕострочный вывод шаблона из лучших позиций
		result << src[i].substr(best_s[i], sample_length) << std::endl;
	result.close();
	return 0;
}