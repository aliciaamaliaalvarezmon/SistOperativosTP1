#include "ConcurrentHashMap.h"
#include "CountWord.h"

using namespace std;

int main(void) {
	ConcurrentHashMap h;
	string p ="aprueba";
	string p2 ="bprueba";
	printf("%d\n", h.member(p) );
	h.addAndInc(p);
	printf("%d\n", h.member(p) );
	h.addAndInc(p,true);
	pair<string, int> max;
	max = h.maximum(1);	
	cout << max.first << " " << max.second << endl;
	h.addAndInc(p2);
	h.addAndInc(p2);
	h.addAndInc(p2);
	printf("%d\n", h.member(p2) );
	max = h.maximum(8);	
	cout << max.first << " " << max.second << endl;
	return 0;
}

