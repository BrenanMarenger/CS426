#include <iostream>           // cout
#include <thread>             // thread
#include <mutex>              // mutex, unique_lock
#include <condition_variable> // condition_variable
#include <unistd.h>			  // usleep
#include <chrono>
#include <string>

using namespace std;


class Buffer {
public:
	int get() { //consumer
		{
			unique_lock<mutex> lk(mtx);
			while (size == 0) {
				cv.wait(lk); //[] {return empty; }
			}
			temp = data[read];
			size--;
			read = (read + 1) % 3;
			cv.notify_all();
			return temp;
		}
	}
	void put(int i) { //producer
		{
			unique_lock<mutex> lk(mtx);
			while (size == 3) {
				cv.wait(lk); //, [] {return full; }
			}
			write = (write + 1) % 3;
			data[write] = i;
			size++;
			cv.notify_all();
		}
	}
private:
	condition_variable cv;
	mutex mtx;
	int data[3]; //0,1,2
	int read = 0;
	int write = -1;
	int size = 0;
	int counter = 0;
	int temp;
};

Buffer buffer;

bool isPrime(int maybe)
{
	if (maybe % 2 == 0)
		return false;
	for (int i = 3; i < maybe; i += 2)
		if (maybe % i == 0)
			return false;
	//cout << "Maybe: " << maybe << endl;
	return true;
	
}
void makePrimes(int start, int stop)
{
	for (int i = start; i < stop; i++)
	{
		if (isPrime(i))
		{
			int s = rand() % 1000;
			usleep(s);
			buffer.put(i);
		}
	}
	buffer.put(-1);
}
void print(int instances)
{
	int stopCounter = 0;
	while (1)
	{
		int value = 0;
		int s = rand() % 100;
		usleep(s);
		value = buffer.get();
		if (value == -1)
		{
			stopCounter = stopCounter + 1;
			if (stopCounter == instances)
			{
				_exit(0);
			}
		}
		if (value != -1)
			cout << "Prime " << value << endl;
	}
}

int main(int argc, char* argv[])
{
	thread th[10];
	int start = 100000001; //100,000,001
	int stop =  100000101; //100,000,101

	for (int i = 0; i < 10; i++) {
		cout << "Thread: " << i << " created!" << endl;
		th[i] = thread(makePrimes, start, stop);
		start += 100;
		stop += 100;
	}
	thread thPrint = thread(print, 10);


	for (int i = 0; i < 10; i++) {
		cout << "Thread: " << i << " done!" << endl;
		th[i].join();
	}
	thPrint.join();

	return 0;
}
