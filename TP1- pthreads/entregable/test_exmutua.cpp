#include "ConcurrentHashMap.h"
#include "CountWord.h"
#include <random>
#include <stdlib.h>
#include <time.h>

using namespace std;

atomic<int> x;


struct prueba{
	ConcurrentHashMap*  h;
	string s;	
};


void * llamada(void* a){
	//Ejemplo donde pasan 2 addinc al mismo tiempo (distinta lista) y despues de que salgan pasan los max
	// 2 addinc, 4 max
	prueba * aux = (prueba*) a;
	int y=0;
	while((y=x.fetch_add(1)) < 6){
		if(y < 3){
			if(y==0){
				aux->s = "calor";
			}
			if(y==1){
				aux->s = "agua";
			}
			if(y==2){
				aux->s = "tuvieja";
			}
			(aux->h)->addAndInc(aux->s,true);
		}else{
			(aux->h)->maximum(3,true);
		}
	}
}

int main(void) {
	ConcurrentHashMap tablita;
	x.store(0);	
	prueba j;
	j.h = &tablita;
	
	pthread_t thread[6];
	int tid;
	for(tid = 0; tid < 6; tid++){
		pthread_create(&thread[tid], NULL, llamada, &j);
	}
	for (tid = 0; tid < 6; ++tid){
        pthread_join(thread[tid], NULL);
   	}  

   	x.store(0);

   	 

	
	return 0;
}