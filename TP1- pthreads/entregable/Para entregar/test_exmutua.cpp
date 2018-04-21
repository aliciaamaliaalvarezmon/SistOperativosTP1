#include "ConcurrentHashMap.h"
#include "CountWord.h"
#include <random>
#include <stdlib.h>
#include <time.h>

using namespace std;

atomic<int> x;

//Estos test se tienen que correr muchas veces porque no se puede definir que thread va a llegar primero a destino. Simplemente es una prueba que quisimos hacer.

struct prueba{
	ConcurrentHashMap*  h;
	string s;	
};

void * llamada(void* a){
	//Ejemplo donde pasan 2 addinc al mismo tiempo (distinta lista) y despues de que salgan pasan los max
	// 3 addinc, 3 max
	//Este test es dificil de que salga porque pasa que los addinc terminan antes la funcion antes que los desalojen
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
				aux->s = "semaforo";
			}
			(aux->h)->addAndInc(aux->s, true);
		}else{
			(aux->h)->maximum(3, true);
		}
	}
}

void* llamada2(void* a){
	//Aca quisimos ver que si pasamos 2 addinc con la misma key, no pasaban los 2 el semaforo al mismo tiempo
	prueba * aux = (prueba*) a;
	int y=0;
	while((y=x.fetch_add(1)) < 6){
		if(y < 3){
			aux->s = "semaforo";
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

   	cout << "---------------------" << endl;


   	pthread_t thread2[6];
	for(tid = 0; tid < 6; tid++){
		pthread_create(&thread2[tid], NULL, llamada2, &j);
	}
	for (tid = 0; tid < 6; ++tid){
        pthread_join(thread2[tid], NULL);
   	} 

   	 

	
	return 0;
}