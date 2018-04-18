#include "ConcurrentHashMap.h"
#include "CountWord.h"
#include <random>
#include <stdlib.h>
#include <time.h>

using namespace std;

int x;

struct prueba{
	ConcurrentHashMap*  h;		
	int i;
	string s;	
};


void * llamada(void* a){
	
	prueba * aux = (prueba*) a;
	
	if(x%2==0){
		if(x==0){
			aux->s = "calor";
		}
		if(x==2){
			aux->s = "agua";
		}
		if(x==4){
			aux->s = "tuvieja";
		}
		if(x==6){
			aux->s = "romi";
		}else{
			aux->s = "papa";
		}
		(aux->h)->addAndInc(aux->s);
	}else{
		(aux->h)->maximum(3);
	}
}

int main(void) {
	ConcurrentHashMap tablita;
	srand(time(NULL));
	prueba j;
	j.h = &tablita;
	j.i = 8;
	
	pthread_t thread[5];
	int tid;
	for(tid = 0; tid < 5; tid++){
		x = rand()%10;
		pthread_create(&thread[tid], NULL, llamada, &j);
	}
	for (tid = 0; tid < 5; ++tid){
        pthread_join(thread[tid], NULL);
   	}  
	
	return 0;
}