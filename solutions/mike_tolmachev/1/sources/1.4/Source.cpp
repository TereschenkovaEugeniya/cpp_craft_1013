#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <cmath>


int main()
{
	std::ifstream in(SOURCE_DIR "/input.txt");
	std::ofstream out(BINARY_DIR "/output.txt");

	if (in.is_open() && out.is_open())
	{
		int N;
		in >> N;

		double code;
		std::set<long long> codes;
		for (int i = 0; i < N; ++i)
		{
			in >> code;
			long long tmp = code*10000.0;
			codes.insert(tmp);
		}

		
		while(in >> code)
		{
			long long tmp = code*10000.0;

			out << ((codes.find(tmp) != codes.end()) ?  "YES" : "NO") << std::endl;
		}
	}

    in.close();
    out.close();
}