#include <chrono>
#include <string>
#include <algorithm>
#include <random>

class stopwatch
{
private: 
	std::chrono::steady_clock::time_point _start, _stop;
	
	int slot = 0;

public:
	
	static const int maxSlots = 5000;

	long long results[maxSlots];

	stopwatch() {
		reset();
	}

	void reset()
	{
		slot = 0;
		memset(results, 0, sizeof(results));
	}

	void start() 
	{
		_start = std::chrono::high_resolution_clock::now();
	}

	long long stop() 
	{
		_stop = std::chrono::high_resolution_clock::now();
		return duration();
	}

	void lap()
	{
		results[slot] = stop();
		slot++;
		start();
	}

	void lap(int index)
	{
		results[index] = stop();
		start();
	}

	long long total()
	{
		long long total = 0;
		for (int i = 0; i < maxSlots; i++)
		{
			total += results[i];
		}

		return total;
	}

	long long average()
	{
		return total() / maxSlots;
	}

	/// <summary>
	/// Microseconds µs
	/// </summary>
	/// <returns></returns>
	long long duration()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start).count();
	}
};

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

void outputResults(stopwatch& watch)
{
	long long total = watch.total();
	long long avg = watch.average();
	puts(string_format("Took %llis (%llims) [%llins]", total / 1000000, total / 1000, total).c_str());
	puts(string_format(" - Average %llis (%llims) [%llins]", avg / 1000000, avg / 1000, avg).c_str());
}

int main(int argc, char** argv) 
{

	stopwatch watch;

	puts("Basic Multiplication");
	puts("Test 1: int");

	std::uniform_int_distribution<int> intUnif(-2147483647, 2147483646);
	std::default_random_engine intRe;

	std::uniform_real_distribution<float> floatUnif(0, 10000);
	std::default_random_engine floatRe;

	std::uniform_real_distribution<double> doubleUnif(0, 10000);
	std::default_random_engine doubleRe;

	std::uniform_real_distribution<long double> longDoubleUnif(0, 10000);
	std::default_random_engine longDoubleRe;


	puts("  -- Warming up..");
	
	int* intNums = new int[1000000];
	for (int i = 0; i < 1000000; i++)
	{
		intNums[i] = intUnif(intRe);
	}
	
	puts("  -- Starting..");

	watch.start();

	for (int r = 0; r < stopwatch::maxSlots; r++)
	{

		int base_int = intNums[r];
		for (int i = 0; i < 1000000; i++)
		{
			int val = intNums[i] * base_int;
		}
		watch.lap();
	}

	watch.stop();
	
	outputResults(watch);

	watch.reset();

	delete[] intNums;

	puts("Test 2: float");
	
	puts("  -- Warming up..");

	float* floatNums = new float[1000000];
	for (int i = 0; i < 1000000; i++)
	{
		floatNums[i] = floatUnif(floatRe);
	}

	puts("  -- Starting..");

	watch.start();

	for (int r = 0; r < stopwatch::maxSlots; r++)
	{
		float base_float = floatNums[r];
		for (int i = 0; i < 1000000; i++)
		{
			float val = floatNums[i] * base_float;
		}
		watch.lap();
	}

	watch.stop();

	outputResults(watch);

	watch.reset();

	delete[] floatNums;
	
	puts("Test 3: double");

	puts("  -- Warming up..");

	double* doubleNums = new double[1000000];
	for (int i = 0; i < 1000000; i++)
	{
		doubleNums[i] = doubleUnif(doubleRe);
	}

	puts("  -- Starting..");

	watch.start();

	for (int r = 0; r < stopwatch::maxSlots; r++)
	{
		double base_double = doubleNums[r];
		for (int i = 0; i < 1000000; i++)
		{
			double val = doubleNums[i] * base_double;
		}
		watch.lap();
	}

	watch.stop();

	outputResults(watch);

	watch.reset();

	delete[] doubleNums;

	puts("Test 4: long double");

	puts("  -- Warming up..");

	long double* lDoubleNums = new long double[1000000];
	for (int i = 0; i < 1000000; i++)
	{
		lDoubleNums[i] = longDoubleUnif(longDoubleRe);
	}

	puts("  -- Starting..");
	
	watch.start();

	for (int r = 0; r < stopwatch::maxSlots; r++)
	{
		long double base_longd = lDoubleNums[r];
		for (int i = 0; i < 1000000; i++)
		{
			long double val = lDoubleNums[i] * base_longd;
		}
		watch.lap();
	}

	watch.stop();

	outputResults(watch);

	watch.reset();

	delete[] lDoubleNums;
	
	puts("Done");

	return 0;
}
