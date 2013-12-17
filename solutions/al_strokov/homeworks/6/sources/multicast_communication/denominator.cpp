#include <denominator.h>

const std::map<const char, double> denominator::denominators = { {
		'A', 10.0 }, { 'B', 100.0 }, { 'C', 1000.0 }, { 'D', 10000.0 }, { 'E',
		100000.0 }, { 'F', 1000000.0 }, { 'G', 10000000.0 },
		{ 'H', 100000000.0 }, { '3', 8.0 }, { '4', 16.0 }, { '5', 32.0 }, { '6',
				64.0 }, { '7', 128.0 }, { '8', 256.0 }, { 'I', 1.0 } };

denominator::denominator() {}

denominator::~denominator() {}


double denominator::getDenominatorByChar(
		const char denominatorChar) {
	if (denominators.count(denominatorChar)) {
		return denominators.at(denominatorChar);
	} else {
		return 1.0;
	}
}
