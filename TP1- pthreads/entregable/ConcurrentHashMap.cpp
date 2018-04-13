#include "ConcurrentHashMap.h"

//template <typename T>


using namespace std;


std::mutex m;
std::mutex m2;
std::mutex m3;
pair<string, int> _maximo("", 0);


ConcurrentHashMap::ConcurrentHashMap(){	
	for(int i = 0; i< 26; i++){		
		Lista<pair<string, int> >* l= new (Lista<pair<string, int> >);				 
		_entradas.push_back(l);
	}
	//_maximo= pair<string, int >("", 0);
	//_ultima  = 0;
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
			m3.lock();//parte critica en ej 6			
			it.Siguiente().second++;
			m3.unlock();				
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



void * ConcurrentHashMap::maxaux( int &ultima){	
		while(ultima < 26){										
			m.lock();
			Lista<pair<string, int>>::Iterador it =(*_entradas[ultima]).CrearIt();
			ultima++;				
			m.unlock();			
 			while(it.HaySiguiente()){	
 				m2.lock();			
				if(it.Siguiente().second >= _maximo.second){						
					_maximo = it.Siguiente();										
				}
				m2.unlock();
				it.Avanzar();
			}				
		}			
		return nullptr;
		
	}


void* max(void* aux){			
	Hashcontador * caux = static_cast<Hashcontador*>(aux);
	//ConcurrentHashMap * conchashmap = static_cast<ConcurrentHashMap*>(aux);
	(caux->h)->maxaux(caux->_ultima);
	return nullptr;
}



pair<string, int> ConcurrentHashMap::maximum(unsigned int nt){		
	int realnt;
	if(nt <= 26){
		realnt = nt;
	}else{
		realnt = 26;
	}	
	Hashcontador aux;
	aux.h = this;
	aux._ultima = 0;
	pthread_t thread[realnt];
	int tid;
	for(tid = 0; tid <  realnt; tid++  ){		
		pthread_create(&thread[tid], NULL, max, &aux );//le pasa a max el struct Hashcontador, con nuestro hash y la thread		
	}
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	}   	
   	return _maximo;
}



ConcurrentHashMap count_words(string arch){
	ConcurrentHashMap h;	
	const char* archivo = arch.c_str();	
	ifstream input;	
	input.open(archivo);	
	string alo;	
	while(!input.eof()){
		input >> alo;		
		h.addAndInc(alo);	
	}	
	return h;	
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

