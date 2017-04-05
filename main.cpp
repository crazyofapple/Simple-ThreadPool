#include <iostream>
#include <fstream>
#include <string>
#include "ThreadPool.h"
using namespace std;

class MyTask : public CTask {
public:
	void run() {
		cout << "okj12112" << endl;
	}
};
int main() {
	ThreadPool tp; 
	tp.Init(1);
	CTask* ptr = new MyTask();
	//sptr->run();
	tp.AddTask(ptr);
	while (1);
	return 0;
}