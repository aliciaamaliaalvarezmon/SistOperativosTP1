#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <mutex>
#include "ListaAtomica.hpp"
#include <vector>
#include <string>
#include <stdio.h>
//template <typename T>
using namespace std;


class ConcurrentHashMap{
private:	
	vector<Lista<pair<string, int> >* > _entradas; //hay 26 letras en el ABC
	//pair<string, int> _maximo; // pair<string, int> maximo("", 0), sino es asi por default va a ver que cambiarlo;	 
	int hash_func(string key){
	 int numero = key[0] - '0'- 49;
		return (numero);
	}

public:	
	ConcurrentHashMap();
	~ConcurrentHashMap();
	void addAndInc(string key);
	bool member(string key);
	void *maxaux(int &ultima);
	pair<string, int> maximum(unsigned int nt);
	ConcurrentHashMap& operator=(const ConcurrentHashMap& TuVieja){	
		while(_entradas.size() > 0){
			delete(_entradas[_entradas.size()-1]);
			_entradas.pop_back();
		}
		for(int i =0; i < 26;  i++){
			//Lista<pair<string, int> >* lant = TuVieja._entradas[i];
			Lista<pair<string, int> >::Iterador it = (*TuVieja._entradas[i]).CrearIt();
			Lista<pair<string, int> >* l = new (Lista<pair<string, int> >);
			while(it.HaySiguiente()){
				(*l).push_front(it.Siguiente());
				it.Avanzar();
			}
			_entradas.push_back(l);
		}			
		return *this;
	}
	

};

struct Hashcontador{
		ConcurrentHashMap* h;
		int _ultima;
	};



ConcurrentHashMap count_words(string arch);

#endif /* HASHMAP_CONCURRENTE_H__ */	