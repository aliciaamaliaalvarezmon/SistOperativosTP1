#include "ConcurrentHashMap.h"
#include "CountWord.h"

using namespace std;

int main(void) {

	ConcurrentHashMap h;
	list<string> l;

	int i = 0;
	while(i < 100){
		l.push_front("corpus");
		i++;
	}

	
	pair<string, int> max;
	max = maximum(3,3,l);
	//Deberia dar volvia 400 (todas las palabras tienen la misma cantidad)
	cout << max.first << " " << max.second << endl;
	max = maximumConcurrente(3,3,l);
	cout << max.first << " " << max.second << endl;	
	
	return 0;
}

