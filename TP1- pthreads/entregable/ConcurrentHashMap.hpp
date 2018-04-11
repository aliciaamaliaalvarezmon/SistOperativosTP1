#ifndef HASHMAP_CONCURRENTE_H__
#define HASHMAP_CONCURRENTE_H__
#include <iostream>
#include <pthread.h>

#include "ListaAtomica.hpp"
//template <typename T>
using namespace std;


class ConcurrentHashMap{
private:
	Lista<pair<string, int>>* _entradas; //hay 26 letras en el ABC
	pair<string, int> _maximo; // pair<string, int> maximo("", 0), sino es asi por default va a ver que cambiarlo;
	
	int hash_func(string key){
		 int numero = key[0] - '0'- 49;
		return (numero);
	}

public:	
	ConcurrentHashMap(){
		_entradas = new( Lista<pair<string, int>> [26]);
	/*	for(int i = 0; i< 26; i++){
			Lista<pair <string, int>>* lista = new(Lista<pair, int>());
			_entradas[i] = Lista<pair<string, int>>();
		}*/
		_maximo = pair<string, int >("", 0);
		
	}

	~ConcurrentHashMap(){
		for(int i = 0; i < 26; i++){
			delete _entradas[i];			
		}
		delete _entradas;

	};
	void addAndInc(string key);
	/*void addAndInc(string key){
		int pos = hash_func(key);		
		Lista<pair<string, int>>::Iterador it =_entradas[pos].CrearIt();
		bool esta = false;
		while(it.HaySiguiente() and !esta){
			if(it.Siguiente().first ==key){
				esta = true;
				it.Siguiente().second++;				
			}else{
				it.Avanzar();
			}

		} 
		if (esta == false){
			_entradas[pos].push_front( pair<string, int>(key, 1));
		}

	}*/

	bool member(string key){
		int pos = hash_func(key);
		Lista<pair<string, int>>::Iterador it =_entradas[pos].CrearIt();
		while(it.HaySiguiente()){
			if(it.Siguiente().first == key){			
				return true;			
			}else{
				it.Avanzar();
			}
		} 
		return false;
	}


	void *maxaux(void *t_num){		
		for(int i= (*((pair<int, int> *) t_num)).first; i < 26; i= i+ (*((pair<int, int> *) t_num)).second ){			
			Lista<pair<string, int>>::Iterador it =_entradas[i].CrearIt();
			while(it.HaySiguiente()){
				if(it.Siguiente().second > _maximo.second){
					_maximo = it.Siguiente(); 
				}
				it.Avanzar();
			}			
		}
		
	}



	pair<string, int> maximum(unsigned int nt){
		int realnt;
		if(nt <= 26){
			realnt = nt;
		}else{
			realnt = 26;
		}		
		pthread_t thread[realnt];
		pair<int, int> tids[realnt];
		int tid;
	

		for(tid = 0; tid <  nt; tid++  ){
			tids[tid].first = tid;
			tids[tid].second = realnt;
			pthread_create(&thread[tid], NULL, maxaux, &tids[tid]);//le pasa a maxaux la thread actual y la cantidad de threads.
		}

		for (tid = 0; tid < realnt; ++tid){
         pthread_join(thread[tid], NULL);
     	}
     	return _maximo;
	}


};

void ConcurrentHashMap::addAndInc(string key){
		int pos = hash_func(key);		
		Lista<pair<string, int>>::Iterador it =_entradas[pos].CrearIt();
		bool esta = false;
		while(it.HaySiguiente() and !esta){
			if(it.Siguiente().first ==key){
				esta = true;
				it.Siguiente().second++;				
			}else{
				it.Avanzar();
			}

		} 
		if (esta == false){
			_entradas[pos].push_front( pair<string, int> (key, 1));
		}

	}



/*




	void *maxaux(void *t_num){
		//version sin concurrrencia nt = 1

		pair<string, int> maximo("", 0);
		for(int i=0; i < 26; i++){			
			Lista<pair<string, int>>::Iterador it =_entradas[i].CrearIt();
			while(it.HaySiguiente()){
				if(it.Siguiente().second > maximo.second){
					maximo = it.Siguiente(); 
				}
				it.Avanzar();
			}			
		}
		return maximo;

	}




*/



























#endif /* HASHMAP_CONCURRENTE_H__ */