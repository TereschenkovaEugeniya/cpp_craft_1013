#include <iostream> 
#include <fstream>
#include <string> 
#include <algorithm>
#include <list>
#include <iomanip>
#include <sstream>
#include <set>

using namespace std;

typedef set<long long> list_of_numbers;

int main() 
{ 
	int N = 0;
	long long number;
	string string_number;
	list_of_numbers keys;
	fstream input_file, output_file;
	stringstream ss;


	input_file.open("Input.txt", fstream::in);
	if(input_file)
	{
		output_file.open("Output.txt", fstream::out);
		input_file >> N;
	
		int i = 0;
		// Читаем число в строку
		
		while(input_file >> string_number)
		{
			i++;
			// Ищем точку, если находим то смотрим сколько символов после точки
			// если их больше 4 то урезаем до 4 симвовлов
			// в противном случае добавляем нули до количества 4 символов после точки
			size_t found = string_number.find(".");
		
			if(found != string::npos)
			{
				if(string_number.size() - found - 1 <= 4)
				{
					string_number.append(4 - (string_number.size() - found - 1), '0');
				}
				else 
				{
					string_number.erase( found + 5, string_number.size() - 1);
				}
				// Удаляем точку
				string_number.erase(string_number.begin() + found);
			}
			else // Число целое поэтому добавляем 4 нуля в конец
			{
				string_number.append(4 , '0');
			}
			// Переводим строку в целое число
			istringstream ( string_number ) >> number;
			// Ключи добавляем во множество
			if( i <= N)
			{
				keys.insert(number);
			}
			else // Если ключи закончились то ищем совпадения
			{
				if(keys.find(number) != keys.end())
				{
					output_file << "YES" <<endl;
				}
				else
				{
					output_file << "NO" <<endl;
				}
			}
		}

		input_file.close();
		output_file.close();
	}
}