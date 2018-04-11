#ifndef HASHMAP_CONCURRENTE_H__
#define HASHMAP_CONCURRENTE_H__
#include <iostream>
#include <pthread.h>

#include "ListaAtomica.hpp"
//template <typename T>
using namespace std;


class ConcurrentHashMap{
private:
	Lista<pair<string, int>>** _entradas; //hay 26 letras en el ABC
	pair<string, int> _maximo; // pair<string, int> maximo("", 0), sino es asi por default va a ver que cambiarlo;
	
	int hash_func(string key){
		 int numero = key[0] - '0'- 49;
		return (numero);
	}

public:	
	ConcurrentHashMap();
	~ConcurrentHashMap();
	void addAndInc(string key);
	bool member(string key);
	void *maxaux(void *t_num);
	pair<string, int> maximum(unsigned int nt);
};

/*void ConcurrentHashMap::addAndInc(string key){
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

*/
#endif /* HASHMAP_CONCURRENTE_H__ */