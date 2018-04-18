#include "ConcurrentHashMap.h"
#include "CountWord.h"

using namespace std;

int main(void) {
	ConcurrentHashMap h;
	list<string> l;
	l.push_front("Prueba.txt");
	l.push_front("Prueba2.txt");
	l.push_front("Prueba3.txt");
	l.push_front("Prueba4.txt");

	pair<string, int> max;
	max = maximum(3,3,l);
	cout << max.first << " " << max.second << endl;
	max = maximumConcurrente(3,3,l);
	cout << max.first << " " << max.second << endl;	
	return 0;
}