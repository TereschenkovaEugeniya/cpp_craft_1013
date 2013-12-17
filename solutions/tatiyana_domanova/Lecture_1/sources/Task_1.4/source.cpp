#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::endl;

char* toString(bool b) {
	if (b) 
		return "YES";
	else 
		return "NO";
}

bool equals(double a, double b) {
	if (a>0 && floor(b) == floor(a) && floor((b-floor(b))*10e3) == floor((a-floor(a))*10e3) )
		return true;
	else if (a<0 && ceil(b) == ceil(a) && ceil((b-ceil(b))*10e3) == ceil((a-ceil(a))*10e3) )
		return true;
	return false;
}

bool contains(double *a, int n, double key) {
	for (int i=0; i<n; i++) {
		if (equals(a[i], key))
			return true;
	}
	return false;
}

int main() {
	ifstream in("input.txt");
	ofstream out("output.txt");
	int n;
	in >> n;
	double *a = new double[n];
	for (int i=0; i<n; i++)
		in >> a[i];
	double key;
	while (!in.eof()) {
		in >> key;
		out << toString(contains(a,n,key)) << endl;
	}
	delete[] a;
	in.close();
	out.close();
	return 0;
}