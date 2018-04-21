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

using namespace std;


class ConcurrentHashMap{
private:
	vector<Lista<pair<string, int> >* > tabla; //hay 26 letras en el ABC sin la ñ		 
	int hash_func(string key){
	 int numero = key[0] - '0'- 49;
		return (numero);
	}

public:

	ConcurrentHashMap();
	~ConcurrentHashMap();
	void mostrarHash();
	void addAndInc(string key, bool test = false); //Los booleanos predefinidos los usamos en el test de exclusion mutua
	bool member(string key);
	Lista<pair <string, int> >* entrada(int i);
	pair<string, int> maximum(unsigned int nt, bool test= false);
	ConcurrentHashMap& operator=(const ConcurrentHashMap& other){	
		while(tabla.size() > 0){
			delete(tabla[tabla.size()-1]);
			tabla.pop_back();
		}
		for(int i =0; i < 26;  i++){			
			Lista<pair<string, int> >::Iterador it = (*other.tabla[i]).CrearIt();
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
		atomic<int> _ultima;
	};

struct Hashescritor{
		ConcurrentHashMap* h;
		string arch;		
	};


struct HashescritorConc{
		ConcurrentHashMap* h;		
		vector<string>* vecti;	
		atomic<int> ult;
	};	

struct HashescritorConc2{
		vector<ConcurrentHashMap* >* h;		
		vector<string>* vecti;	
		atomic<int>	ult_pos;		
	};

#endif /* HASHMAP_CONCURRENTE_H__ */	