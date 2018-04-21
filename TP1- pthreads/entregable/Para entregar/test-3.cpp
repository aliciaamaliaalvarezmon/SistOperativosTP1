#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.h"
#include "CountWord.h"

using namespace std;

int main(int argc, char **argv) {
	ConcurrentHashMap h;
	list<string> l = { "corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4" };
	int i;

	if (argc != 2) {
		cerr << "uso: " << argv[0] << " #threads" << endl;
		return 1;
	}
	h = count_words3(atoi(argv[1]), l);
	for (i = 0; i < 26; i++) {
		for (auto it = h.entrada(i)->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
			auto t = it.Siguiente();
			cout << t.first << " " << t.second << endl;
		}
	}

	return 0;
}

