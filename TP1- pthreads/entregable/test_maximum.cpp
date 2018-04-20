#include "ConcurrentHashMap.h"
#include "CountWord.h"

using namespace std;

int main(void) {

	struct timespec start,stop;

	clock_gettime(CLOCK_REALTIME,&start);
	clock_gettime(CLOCK_REALTIME, &stop);

	double aux1=0;

	ConcurrentHashMap h;
	list<string> l;

	int i = 0;
	while(i < 50){
		l.push_front("corpus");
		i++;
	}

	i = 1;
	while(i <= 50){
		int j = 0;
		while(j <100){

			clock_gettime(CLOCK_REALTIME,&start);
			maximumConcurrente(3,i,l);
			clock_gettime(CLOCK_REALTIME, &stop);
			aux1 = aux1 + ((stop.tv_sec - start.tv_sec) + ((stop.tv_nsec - start.tv_nsec)*(0.000000001)));
			j++;
		}
		cout << (aux1/100)<< endl;
		i++;
	}

	/*
	//pair<string, int> max;
	//max = 
	clock_gettime(CLOCK_REALTIME,&start);
	maximum(3,3,l);
	clock_gettime(CLOCK_REALTIME, &stop);
	cout << (stop.tv_sec - start.tv_sec) + ((stop.tv_nsec - start.tv_nsec)*(0.000000001)) << endl;
	//cout << max.first << " " << max.second << endl;
	//max = 
	clock_gettime(CLOCK_REALTIME,&start);
	maximumConcurrente(3,3,l);
	clock_gettime(CLOCK_REALTIME, &stop);
	cout << (stop.tv_sec - start.tv_sec) + ((stop.tv_nsec - start.tv_nsec)*(0.000000001)) << endl;
	//cout << max.first << " " << max.second << endl;	
	*/
	return 0;
}

