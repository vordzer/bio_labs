#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//Функция поиска медианы одного шаблона
unsigned find_median(std::vector<std::string> &src, std::string templ)
{
	unsigned err_word = 0, min_err_word = -1, err_all = 0;
	for (unsigned i = 0; i < src.size(); ++i) // Цикл по всем цепочкам
	{
		for (unsigned j = 0; j + templ.size() < src[i].size(); ++j) // Цикл перебора возможных индексов внутри одной цепочки
		{
			for (unsigned k = 0; k < templ.size(); ++k) // Цикл проверки совпадений внутри шаблона
				if (templ[k] != src[i][j + k]) err_word++; // При несовпадении нуклеотида в шаблоне и в цепочке, увеличивается счётчик ошибок
			if(min_err_word > err_word) min_err_word = err_word; // Наименьший результат записывается в минимум по данной строке
			err_word = 0;
		}
		err_all += min_err_word; // Сбор минимумов по каждой строке
		min_err_word = -1;
	}
	return err_all;
}

// Функция генерации шаблонов и поиска медиан
void search_best_median(std::vector<std::string> &src, std::string &best_word, unsigned sample_length, unsigned &min_median, std::string str = "")
{
	if(str.size() + 1 == sample_length) // Если остался неизвестным последний нуклеотид то из них выбираем лучшие для записи в best_word
	{
		unsigned cur_median;
		cur_median = find_median(src, str+"A");
		if(cur_median < min_median)
		{
			best_word = str+"A";
			min_median = cur_median;
		}
		cur_median = find_median(src, str+"C");
		if(cur_median < min_median)
		{
			best_word = str+"C";
			min_median = cur_median;
		}
		cur_median = find_median(src, str+"G");
		if(cur_median < min_median)
		{
			best_word = str+"G";
			min_median = cur_median;
		}
		cur_median = find_median(src, str+"T");
		if(cur_median < min_median)
		{
			best_word = str+"T";
			min_median = cur_median;
		}
	}
	else // Если неизвестны несколько символов, для ближайшего запускаем рекурсивную функцию заполнения
	{
		if(find_median(src, str+"A") <= min_median) // Проверка на текущую медиану, если превысила, то не проверяем эту ветку дальше, делаем bypass
			search_best_median(src, best_word, sample_length, min_median, str + "A");
		if(find_median(src, str+"C") <= min_median) // Проверка на текущую медиану, если превысила, то не проверяем эту ветку дальше, делаем bypass
			search_best_median(src, best_word, sample_length, min_median, str + "C");
		if(find_median(src, str+"G") <= min_median) // Проверка на текущую медиану, если превысила, то не проверяем эту ветку дальше, делаем bypass
			search_best_median(src, best_word, sample_length, min_median, str + "G");
		if(find_median(src, str+"T") <= min_median) // Проверка на текущую медиану, если превысила, то не проверяем эту ветку дальше, делаем bypass
			search_best_median(src, best_word, sample_length, min_median, str + "T");
	}
}

int main()
{
	std::ifstream fsrc("input.txt");
	if (!fsrc.is_open()) //Проверка существования войда ввода
	{
		std::cout << "Input file not found\n";
		return 1;
	}
	std::vector<std::string> src; // Вектор для хранения всех заданных цепочек
	unsigned sample_length;
	fsrc >> sample_length;// Вычитывание длины шаблона
	if(sample_length == 0) // Проверка на нулевой шаблон, создаётся пустой ответ.
	{
		std::ofstream result("output.txt");
		result.close();
		return 0;
	}
	std::string cur_line;
	while (fsrc.good()) // Вычитывается по одной строке и записывается в вектор до достижения конца файла (в этот раз нет количества строк, приходится так)
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
	std::string best_word; // Лучшее сочетание
	unsigned min_median = -1; // Стартовые максимальные данные
	search_best_median(src, best_word, sample_length, min_median); // генерация шаблонов и поиск
	std::ofstream result("output.txt");
	result << best_word << std::endl;
	result.close();
	return 0;
}