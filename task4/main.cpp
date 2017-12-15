#include<fstream>
#include<iostream>
#include<string>
#include<vector>

//Функция поиска медианы одного шаблона
unsigned find_median(std::vector<std::string> &src, std::string &templ)
{
	unsigned err_word = 0, min_err_word = -1, err_all = 0;
	for (unsigned i = 0; i < src.size(); ++i) // Цикл по всем цепочкам
	{
		for (unsigned j = 0; j + templ.size() < src[i].size(); ++j) // Цикл перебора возможных индексов внутри одной цепочки
		{
			for (unsigned k = 0; k < templ.size(); ++k) // Цикл проверки совпадений внутри шаблона
				if (templ[k] != src[i][j + k]) err_word++; // При несовпадении нуклеотида в шаблоне и в цепочке, увеличивается счётчик ошибок
			if (min_err_word > err_word) min_err_word = err_word; // Наименьший результат записывается в минимум по данной строке
			err_word = 0;
		}
		err_all += min_err_word; // Сбор минимумов по каждой строке
		min_err_word = -1;
	}
	return err_all;
}

// Функция генерации всех возможных шаблонов
void fill_vec_tmpl(std::vector<std::string> &vec_tmpl, unsigned sample_length, std::string str = "")
{
	if (str.size() + 1 == sample_length) // Если остался неизвестным последний нуклеотид, записываем в шаблоны 4 варианта
	{
		vec_tmpl.push_back(str + "A");
		vec_tmpl.push_back(str + "C");
		vec_tmpl.push_back(str + "G");
		vec_tmpl.push_back(str + "T");
	}
	else // Если неизвестны несколько символов, для ближайшего запускаем рекурсивную функцию заполнения
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
	if (!fsrc.is_open()) //Проверка существования войда ввода
	{
		std::cout << "Input file not found\n";
		return 1;
	}
	std::vector<std::string> src; // Вектор для хранения всех заданных цепочек
	unsigned sample_length;
	fsrc >> sample_length;// Вычитывание длины шаблона
	if (sample_length == 0) // Проверка на нулевой шаблон, создаётся пустой ответ.
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
	std::vector<std::string> vec_shab; // Вектор для хранения всех возможных шаблонов
	fill_vec_tmpl(vec_shab, sample_length); // генерация всех шаблонов
	unsigned min_index = 0, min_median = find_median(src, vec_shab[0]); // Анализируется первый элемент для дальнейшего сравнения
	for (unsigned i = 1; i < vec_shab.size(); ++i)  // Для каждого шаблона считается медиана и сравнивается с минимальной.
	{
		unsigned cur_median = find_median(src, vec_shab[i]);
		if (cur_median < min_median)
		{
			min_median = cur_median;
			min_index = i; // Сохранения минимального индекса для выбора минимального шаблона
		}
	}
	std::ofstream result("output.txt");
	result << vec_shab[min_index] << std::endl;
	result.close();
	return 0;
}