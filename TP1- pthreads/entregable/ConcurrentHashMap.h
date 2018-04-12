#ifndef CONCURRENT_HASH_MAP_H__
#define CONCURRENT_HASH_MAP_H__
#include <iostream>
#include <pthread.h>
#include <mutex>
#include "ListaAtomica.hpp"
#include <vector>
#include <string>
#include <studio.h>
//template <typename T>
using namespace std;


class ConcurrentHashMap{
private:
	/*Lista<pair<string, int>>** _entradas; //hay 26 letras en el ABC
	pair<string, int> _maximo; // pair<string, int> maximo("", 0), sino es asi por default va a ver que cambiarlo;
	int _ultima;
*/
	vector<Lista<pair<string, int> >* > _entradas; //hay 26 letras en el ABC
	pair<string, int> _maximo; // pair<string, int> maximo("", 0), sino es asi por default va a ver que cambiarlo;	
    int  _ultima; 	
	int hash_func(string key){
	 int numero = key[0] - '0'- 49;
		return (numero);
	}

public:	
	ConcurrentHashMap();
	~ConcurrentHashMap();
	void addAndInc(string key);
	bool member(string key);
	void *maxaux();
	pair<string, int> maximum(unsigned int nt);

};
#endif /* HASHMAP_CONCURRENTE_H__ */