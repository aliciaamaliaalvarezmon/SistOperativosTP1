#include "ConcurrentHashMap.h"

using namespace std;

int main(void) {
	ConcurrentHashMap h;
	string p ="aprueba";
	string p2 ="bprueba";
	list<string> l;
	//l.push_front("sarasa.txt");
	//l.push_front("segunto.txt");
	l.push_front("Prueba.txt");
	l.push_front("Prueba2.txt");
	l.push_front("Prueba3.txt");
	l.push_front("Prueba4.txt");
	//printf("%d\n", h.member(p) );
	h.addAndInc(p);
	//printf("%d\n", h.member(p) );
	h.addAndInc(p);
	pair<string, int> max;
	//max = h.maximum(1);	
	//cout << max.first << " " << max.second << endl;
	h.addAndInc(p2);
	h.addAndInc(p2);
	h.addAndInc(p2);
	//printf("%d\n", h.member(p2) );
	//max = h.maximum(8);	
	//cout << max.first << " " << max.second << endl;
	//h = count_words("sarasa.txt");
	//h.mostrarHash();
	//h = count_words2(l);
	//h.mostrarHash();
	h = count_words3(4, l);	
	h.mostrarHash();
	return 0;
}

