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
#include <list>
//template <typename T>
using namespace std;


class ConcurrentHashMap{
//private:
public:	
	vector<Lista<pair<string, int> >* > tabla; //hay 26 letras en el ABC
	//pair<string, int> _maximo; // pair<string, int> maximo("", 0), sino es asi por default va a ver que cambiarlo;	 
	int hash_func(string key){
	 int numero = key[0] - '0'- 49;
		return (numero);
	}

//public:

	ConcurrentHashMap();
	~ConcurrentHashMap();
	void addAndInc(string key);
	bool member(string key);
	void *maxaux(int &ultima);
	pair<string, int> maximum(unsigned int nt);
	ConcurrentHashMap& operator=(const ConcurrentHashMap& TuVieja){	
		while(tabla.size() > 0){
			delete(tabla[tabla.size()-1]);
			tabla.pop_back();
		}
		for(int i =0; i < 26;  i++){
			//Lista<pair<string, int> >* lant = TuVieja._entradas[i];
			Lista<pair<string, int> >::Iterador it = (*TuVieja.tabla[i]).CrearIt();
			Lista<pair<string, int> >* l = new (Lista<pair<string, int> >);
			while(it.HaySiguiente()){
				(*l).push_front(it.Siguiente());
				it.Avanzar();
			}
			tabla.push_back(l);
		}			
		return *this;
	}
	

};

struct Hashcontador{
		ConcurrentHashMap* h;
		int _ultima;
	};

struct Hashescritor{
		ConcurrentHashMap* h;
		string arch;
	};	



ConcurrentHashMap count_words(string arch);
ConcurrentHashMap count_words2(list<string>archs);

#endif /* HASHMAP_CONCURRENTE_H__ */	