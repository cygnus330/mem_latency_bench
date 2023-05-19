#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>
#include <cstdint>
#include <random>

using namespace std;
using namespace chrono;

typedef high_resolution_clock HRtimer;
typedef int64_t Int;

struct link_List
{
	Int Val;
	link_List* Next = nullptr;
};

static_assert(sizeof(link_List) == 16, "not 64-bit env");

double f(const Int N, const Int P)
{
	auto v = (link_List*)malloc(sizeof(link_List) * N);
	auto* p = (link_List**)malloc(sizeof(link_List*) * N);

	for(int i = 0; i < N; i++)
		v[i].Val = i;

	for(Int i = 0; i < N; i++)
		p[i] = &v[i];

	shuffle(p, p + N, mt19937(random_device()()));
	for(Int i = 0; i < N - 1; i++)
		p[i]->Next = p[i + 1];

	auto* startPtr = p[0];
	auto* lastPtr = p[N - 1];
	free(p);

	auto start_time = HRtimer::now();

	for(Int i = 0; i < P; i++)
	{
		auto* ptr = startPtr;
		while(ptr != lastPtr)
			ptr = ptr->Next;
	}

	free(v);

	auto end_time = HRtimer::now();
	Int delta_time = duration_cast<nanoseconds>(end_time - start_time).count();

	return (double)delta_time / (double)(P * N);
}

void memPrint(const Int size, const double time)
{
	vector<string> v = {"B", "KB", "MB", "GB", "TB"};
	Int idx = 0, cvtSize = size;
	while(cvtSize > (1 << 10))
	{
		idx++;
		cvtSize >>= 10;
	}
	double cvtSizeFloat = size / pow(1024, idx);
	printf("%.04lf ", cvtSizeFloat);
	cout << v[idx] << " " << time << " ns" << endl;

	ofstream file;
	file.open("log.txt", ios::app);
	file << size << "\t" << time << endl;
}

int main(int argc, const char* argv[])
{
	Int minMem = stoll(argv[1]);
	Int maxMem = stoll(argv[2]);

	ofstream* file = new ofstream;
	file->open("log.txt", ios::out);
	file->close();
	delete file;

	const Int maxMemAccess = 5 * 1e8;
	Int memSize = ((Int)1 << minMem);
	Int maxMemSize = ((Int)1 << maxMem);
	while(memSize <= maxMemSize)
	{
		Int P = max((Int)1, (Int)(maxMemAccess / memSize));
		double T = f(memSize / 16, P);
		memPrint(memSize, T);

		memSize = (Int)(memSize * (double)1.25);
	}
	memPrint(maxMemSize, f(maxMemSize / 16, max((Int)1, (Int)(maxMemAccess / maxMemSize))));

	return 0;
}
