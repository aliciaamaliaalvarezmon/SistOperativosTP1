#include "ConcurrentHashMap.h"

//template <typename T>



using namespace std;

std::atomic<int> maxi(0);
std::atomic<int> addi(0);
int nada = 0;

//mutex posicion[26];
mutex m;
mutex m2;
pthread_mutex_t posicion[26];
pair<string, int> _maximo("", 0);
//_entradas

ConcurrentHashMap::ConcurrentHashMap(){	
	for(int i = 0; i< 26; i++){		
		Lista<pair<string, int> >* l= new (Lista<pair<string, int> >);				 
		tabla.push_back(l);
	}
	//_maximo= pair<string, int >("", 0);
	//_ultima  = 0;
	//void* operator new[] (std::size_t size);
}

ConcurrentHashMap::~ConcurrentHashMap(){		
	for(int i = 0; i < 26; i++){			
		delete(tabla[i]);		
	}	
}



void ConcurrentHashMap::addAndInc(string key){
	while(!maxi.compare_exchange_weak(nada, maxi)){
	}
	addi++;	
	int pos = hash_func(key);	
	Lista<pair<string, int>>::Iterador it = (*tabla[pos]).CrearIt();
	bool esta = false;	
	while(it.HaySiguiente() and !esta){
		if(it.Siguiente().first ==key){									
			esta = true;
			pthread_mutex_lock(&posicion[pos]);					
			it.Siguiente().second++;
			pthread_mutex_unlock(&posicion[pos]);								
		}else{
			it.Avanzar();
		}
	} 
	if (esta == false){
		//parte critica en ej 6		
		pthread_mutex_lock(&posicion[pos]);	
		(*tabla[pos]).push_front(pair<string, int>(key, 1));
		pthread_mutex_unlock(&posicion[pos]);
	}	
	addi--;
}

bool ConcurrentHashMap::member(string key){
	int pos = hash_func(key);	
	Lista<pair<string, int>>::Iterador it =(*tabla[pos]).CrearIt();
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
			Lista<pair<string, int>>::Iterador it =(*tabla[ultima]).CrearIt();
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
	maxi++;
	while(!addi.compare_exchange_weak(nada, addi)){
	}	
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
   	maxi--;     	 	
   	return _maximo;
   	//maxi--; 
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


void * count_wordsaux(void* aux){
	Hashescritor * caux = static_cast<Hashescritor*>(aux);	
	const char* archivo = (caux)->arch.c_str();
	ifstream input;
	input.open(archivo);
	string alo;
	while(!input.eof()){
		input >> alo;
		(caux->h)->addAndInc(alo); 
	}
	
	return nullptr;	
}


ConcurrentHashMap count_words2(std::list<string>archs){
	ConcurrentHashMap escri;
	Hashescritor aux;
	aux.h = &escri;		
	auto it = std::begin(archs);	
	int cantarchivos = archs.size();	
	pthread_t thread[cantarchivos];
	int tid;
	for(tid = 0; tid <  cantarchivos; tid++  ){	
		aux.arch = (*it);	
		pthread_create(&thread[tid], NULL, count_wordsaux, &aux);//le pasa a max el struct Hashcontador, con nuestro hash y la thread		
		++it;
	}
	for (tid = 0; tid < cantarchivos; ++tid){
        pthread_join(thread[tid], NULL);
   	}
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

