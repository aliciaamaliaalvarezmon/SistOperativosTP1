#ifndef HASHMAP_CONCURRENTE_H__
#define HASHMAP_CONCURRENTE_H__
#include <iostream>
#include "ListaAtomica.hpp"

//template <typename T>
using namespace std;

class ConcurrentHashMap{
private:
	Lista<pair<string, int>> _entradas[26]; //hay 26 letras en el ABC
	//Lista<pair<string, int>>* _inicio;

int hash_func(string key){
	 int numero = key[0] - '0'- 49;
	return (numero);
}

public:	
	/*ConcurrentHashMap(){
		for(int i = 0; i < 26; i++){			
			_entradas[i] = (new (Lista<pair<sting, int>>()));
		}
	}

	~ConcurrentHashMap();*/

	void addAndInc(string key){
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

	}

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

	pair<string, unsigned int>maximum(unsigned int nt){
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







};











































#endif /* HASHMAP_CONCURRENTE_H__ */