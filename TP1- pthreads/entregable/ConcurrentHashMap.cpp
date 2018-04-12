#include "ConcurrentHashMap.h"
#include <mutex>

//template <typename T>


using namespace std;


std::mutex m;
std::mutex m2;

/*ConcurrentHashMap::ConcurrentHashMap(){
	_entradas = new (Lista<pair<string, int>>*[26]);
	cout << (*_entradas).size() << endl;
	for(int i = 0; i< 26; i++){
		Lista<pair <string, int>>* lista = new (Lista<pair <string, int>>);
		_entradas[i] = lista;		
	}
	_maximo = pair<string, int >("", 0);
	_ultima = 0;
}

ConcurrentHashMap::~ConcurrentHashMap(){
	for(int i = 0; i < 26; i++){
		delete _entradas[i];			
	}
	delete _entradas;
}*/
ConcurrentHashMap::ConcurrentHashMap(){	
	for(int i = 0; i< 26; i++){		
		Lista<pair<string, int> >* l= new (Lista<pair<string, int> >);				 
		_entradas.push_back(l);
	}
	_maximo= pair<string, int >("", 0);
	_ultima  = 0;
	//void* operator new[] (std::size_t size);
}

ConcurrentHashMap::~ConcurrentHashMap(){
	for(int i = 0; i < 26; i++){
		delete(_entradas[i]);
	}
}



void ConcurrentHashMap::addAndInc(string key){	
	int pos = hash_func(key);	
	Lista<pair<string, int>>::Iterador it = (*_entradas[pos]).CrearIt();
	bool esta = false;
	while(it.HaySiguiente() and !esta){
		if(it.Siguiente().first ==key){			
			esta = true;
			//parte critica en ej 6			
			it.Siguiente().second++;				
		}else{
			it.Avanzar();
		}
	} 
	if (esta == false){
		//parte critica en ej 6
		(*_entradas[pos]).push_front(pair<string, int>(key, 1));
	}

}

bool ConcurrentHashMap::member(string key){
	int pos = hash_func(key);	
	Lista<pair<string, int>>::Iterador it =(*_entradas[pos]).CrearIt();
	while(it.HaySiguiente()){
		if(it.Siguiente().first == key){						
			return true;			
		}else{
			it.Avanzar();
		}
	} 
	return false;
}



void * ConcurrentHashMap::maxaux(){		
		while(_ultima < 26){										
			m.lock();
			Lista<pair<string, int>>::Iterador it =(*_entradas[_ultima]).CrearIt();
			_ultima++;				
			m.unlock();			
 			while(it.HaySiguiente()){				
				if(it.Siguiente().second >= _maximo.second){					
					m2.lock();
					_maximo = it.Siguiente();
					m2.unlock();					
				}
				it.Avanzar();
			}				
		}			
		return nullptr;
		
	}


	void* max(void* aux){	
		ConcurrentHashMap * conchashmap = static_cast<ConcurrentHashMap*>(aux);
		conchashmap->maxaux();
		return nullptr;

	}

	pair<string, int> ConcurrentHashMap::maximum(unsigned int nt){		
		int realnt;
		if(nt <= 26){
			realnt = nt;
		}else{
			realnt = 26;
		}		
		pthread_t thread[realnt];
		//pair<int, int> tids[realnt];
		int tid;
		for(tid = 0; tid <  realnt; tid++  ){
			//tids[tid].first = tid;
			//tids[tid].second = realnt;
			pthread_create(&thread[tid], NULL, max, this);//le pasa a maxaux la thread actual y la cantidad de threads.
		}

		for (tid = 0; tid < realnt; ++tid){
         pthread_join(thread[tid], NULL);
     	}
     	_ultima = 0;
     	return _maximo;
	}


/*

pair<string, int> ConcurrentHashMap::maximum(unsigned int nt){
		//version sin concurrrencia nt = 1

		pair<string, int> maximo("", 0);
		for(int i=0; i < 26; i++){			
			Lista<pair<string, int>>::Iterador it =(*_entradas[i]).CrearIt();
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
