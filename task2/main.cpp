#include<map>
#include<fstream>
#include<iostream>
#include<string>

int main()
{
	std::ifstream fsrc("source.txt");
	if (!fsrc.is_open()) //check for exist source file
	{
		std::cout << "Source file not found\n";
		return 1;
	}
	std::string src;
	unsigned length_word;
	fsrc >> src; // read first line
	fsrc >> length_word;
	fsrc.close();
	if (src.size() < length_word) //check length
	{
		std::cout << "Source line too short\n";
		return 2;
	}
	std::map<const std::string, unsigned> dict; //dictionary for all word
	std::string cur_word{ src.substr(0, length_word) }; //first word
	auto p = src.begin() + length_word; //iterator for new position
	while (p != src.end())
	{
		auto it = dict.find(cur_word);
		if (it != dict.end()) it->second++; // increment count for this word
		else dict.emplace(cur_word, 1); // create new word
		cur_word.push_back(*p++); // change word add next symbol
		cur_word.erase(0, 1); // change word delete first symbol
	}
	std::ofstream result("result.txt");
	std::string res;
	unsigned max = 0;
	for (auto &it : dict) // check all element in map
	{
		if (it.second == max) res += it.first + " "; // if equal maximum add word in result
		else if (it.second > max) // if new maximum clear result and write new word
		{
			res.clear();
			res += it.first + " ";
			max = it.second;
		}
	}
	result << res << std::endl; // write to output file
	result.close();
	return 0;
}