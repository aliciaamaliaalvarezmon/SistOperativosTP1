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

	h = count_words("Prueba.txt");
	h.mostrarHash();
	cout << "------------------------------------------" << endl;
	h = count_words2(l);
	h.mostrarHash();
	cout << "------------------------------------------" << endl;
	h = count_words3(3,l);
	h.mostrarHash();
	
	return 0;
}