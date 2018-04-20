#include "ConcurrentHashMap.h"
#include "CountWord.h"



using namespace std;



mutex m2;
mutex posicion[26];
pair<string, int> _maximo("", 0);



ConcurrentHashMap::ConcurrentHashMap(){	
	for(int i = 0; i< 26; i++){		
		Lista<pair<string, int> >* l= new (Lista<pair<string, int> >);				 
		tabla.push_back(l);
	}
}

ConcurrentHashMap::~ConcurrentHashMap(){	
	for(int i = 0; i < 26; i++){			
		delete(tabla[i]);		
	}	
}



void ConcurrentHashMap::addAndInc(string key, bool test){
	int pos = hash_func(key);
	if(test){	
		cout << "entro un add" << " clave: " << key << endl;
	}
	posicion[pos].lock();
	if(test){
		cout << "paso un add" << " clave: " << key << endl;
	}
	Lista<pair<string, int>>::Iterador it = (*tabla[pos]).CrearIt();	
	bool esta = false;				
	while(it.HaySiguiente() and !esta){
		if(it.Siguiente().first ==key){												
			esta = true;							
			it.Siguiente().second++;
			break;											
		}else{
			it.Avanzar();
		}
	} 	
	if (esta == false){
		(*tabla[pos]).push_front(pair<string, int>(key, 1));
	}
	if(test){
		cout << "va a salir un add" << " clave: " << key << endl;
	}
	posicion[pos].unlock();
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

Lista<pair <string, int> >* ConcurrentHashMap::entrada(int i){
	return tabla[i];
}

void * maxola(void* aux){	
		Hashcontador* caux = (Hashcontador*)aux;
		int x;
		while((x = (caux->_ultima).fetch_add(1)) < 26){
			Lista<pair<string, int>>::Iterador it =(*((caux->h)->entrada(x))).CrearIt();												
 			while(it.HaySiguiente()){
 				m2.lock();	 							
				if((it.HaySiguiente()) and (it.Siguiente().second >= _maximo.second)){						
					_maximo = it.Siguiente();										
				}
				m2.unlock();						
				it.Avanzar();				
								
			}						
		}			
		return nullptr;
		
}

pair<string, int> ConcurrentHashMap::maximum(unsigned int nt, bool test){
	for(int i = 0; i < 26; i++){
		posicion[i].lock();
	}	
	int realnt;
	if(nt <= 26){
		realnt = nt;
	}else{
		realnt = 26;
	}
	Hashcontador aux;
	aux.h = this;
	aux._ultima.store(0);
	pthread_t thread[realnt];
	int tid;	
	for(tid = 0; tid <  realnt; tid++  ){
		pthread_create(&thread[tid], NULL, maxola, &aux );//le pasa a max el struct Hashcontador, con nuestro hash y la thread		
	}
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	} 
  	for(int i = 0; i < 26; i++){
		posicion[i].unlock();
	}	 	 	
   	return _maximo;   	
}

void ConcurrentHashMap::mostrarHash(){
	for(int i = 0; i < 26; i++){
		Lista<pair<string, int>>::Iterador it =(*tabla[i]).CrearIt();
		while(it.HaySiguiente()){
			cout << "<" << it.Siguiente().first << ", " << it.Siguiente().second << "> ";
			it.Avanzar();
		}
		cout << endl;
	}
}

ConcurrentHashMap count_words(string arch){
	ConcurrentHashMap h;	
	const char* archivo = arch.c_str();	
	ifstream input;	
	input.open(archivo);	
	string alo;	
	while(!input.eof()){
		input >> alo;
		//Esto es por si el archivo termina con un salto de linea (new line)		
		input.ignore();
		if(input.peek() == 10){
			break;
		}
		h.addAndInc(alo);	
	}	
	return h;	
}


void * count_wordsaux(void* aux){
	Hashescritor * caux = (Hashescritor*) aux;	
	const char* archivo = (caux)->arch.c_str();
	ifstream input;
	input.open(archivo);
	string alo;
	while(!input.eof()){
		input >> alo;
		input.ignore();
		if(input.peek() == 10){
			break;
		}
		(caux->h)->addAndInc(alo); 
	}		
	return nullptr;	
	
}



ConcurrentHashMap count_words2(std::list<string>archs){
	ConcurrentHashMap escri;
	int cantarchivos = archs.size();	
	Hashescritor* separador[cantarchivos];				
	auto it = std::begin(archs);		
	pthread_t thread[cantarchivos];
	int tid;
	for(tid = 0; tid <  cantarchivos; tid++  ){	
		separador[tid] = new (Hashescritor);
		separador[tid]->h = &escri;
		separador[tid]->arch = (*it);			
		pthread_create(&thread[tid], NULL, count_wordsaux, separador[tid]);//le pasa a max el struct Hashcontador, con nuestro hash y la thread		
		++it;
	}		
	for (tid = 0; tid < cantarchivos; ++tid){
        pthread_join(thread[tid], NULL);
   	}  
   	for(tid = 0; tid <  cantarchivos; tid++  ){	
		delete(separador[tid]);		
	}		 
   	return escri;
}



void * count_words_limthreads_aux(void* aux){
	HashescritorConc * caux = (HashescritorConc*) aux;
	int x;
	while((x = (caux->ult).fetch_add(1)) < ((caux->vecti)->size())){
		const char* archivo = (*(caux)->vecti)[x].c_str();
		ifstream input;				
		input.open(archivo);
		string alo;
		while(!input.eof()){			
			input >> alo;	
			input.ignore();
			if(input.peek() == 10){
				break;
			}		
			(caux->h)->addAndInc(alo);
		}		
		input.close();		
	}			
	return nullptr;	
	
}


ConcurrentHashMap count_words3(unsigned int n, list<string> archs){
	int realnt;
	if(n <= archs.size()){
		realnt = n;
	}else{
		realnt = archs.size();
	}	
	ConcurrentHashMap escri;
	vector<string> archivador;
	list<string>::iterator	iteaux = archs.begin();
	while(iteaux != archs.end()){
		archivador.push_back((*iteaux));
		iteaux++;
	}
	//tenemos vector  archivador con los archivos de lista 	
	HashescritorConc* separador = new (HashescritorConc);
	separador->h = &escri;
	separador->vecti = &archivador;
	separador->ult.store(0);//ultima posicion de vector			
	pthread_t thread[realnt];
	int tid;
	for(tid = 0; tid <  realnt; tid++  ){
		pthread_create(&thread[tid], NULL, count_words_limthreads_aux, separador);//le pasa a max el struct Hashcontador, con nuestro hash y la thread	
	}			
	for (tid = 0; tid < realnt; tid++){
        pthread_join(thread[tid], NULL);
   	}  
   	delete(separador);    
   	return escri;

	
}




void * count_words_limthreads_aux2(void* aux){	
	HashescritorConc2 * caux = (HashescritorConc2*) aux;
	int x;
	while( (x = (caux->ult_pos).fetch_add(1)) < ((caux->vecti)->size())){	
		string archivo = (*(caux)->vecti)[x];
		ConcurrentHashMap* hash_actual = (*(caux->h))[x];	
		(*hash_actual) = count_words(archivo);		
	}
	return nullptr;	
}


pair<string, unsigned int> maximum(unsigned int p_archivos, unsigned int p_maximos, list<string>archs){	
	int realnt;
	if(p_archivos <= archs.size()){
		realnt = p_archivos;
	}else{
		realnt = archs.size();
	}		
	vector<ConcurrentHashMap* > escri;	
	for (int i = 0; i < archs.size(); ++i){
        ConcurrentHashMap* nuevi = new (ConcurrentHashMap); 
        escri.push_back(nuevi);
   	}
   	vector<string> archivador;
	list<string>::iterator	iteaux = archs.begin();
	while(iteaux != archs.end()){
		archivador.push_back((*iteaux));
		iteaux++;
	}   
	//aqui archivador tiene la lista archs		
	HashescritorConc2* separador = new (HashescritorConc2);
	separador->h = &escri;
	separador->vecti = &archivador;
	separador->ult_pos.store(0);//ultimo archivo abierto
	
	pthread_t thread[realnt];
	int tid;
	for(tid = 0; tid < realnt; tid++){
		pthread_create(&thread[tid], NULL, count_words_limthreads_aux2, separador);
	}
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	}    
   ConcurrentHashMap hash_recolector;
   int veces;
   for(int i =0; i < escri.size(); i++){
   	for(int j = 0; j< 26; j++){
   		Lista<pair< string, int> >::Iterador it = (escri[i]->entrada(j))->CrearIt();
   		while(it.HaySiguiente()){
   			veces = it.Siguiente().second;
   			while(veces > 0){   			
   				hash_recolector.addAndInc(it.Siguiente().first);
   				veces--;
   			}
   			it.Avanzar();
   		}
   	}
   }   	
   	pair<string, int> supermax = hash_recolector.maximum(p_maximos);   	 	
  	for (int i = 0; i < archs.size(); ++i){       
        delete(escri[i]);
   	}
   	return supermax;
}



pair<string, unsigned int> maximumConcurrente(unsigned int p_archivos, unsigned int p_maximos, list<string>archs){	
	ConcurrentHashMap  escri = count_words3(p_archivos, archs);   	
   	pair<string, int> supermax = escri.maximum(p_maximos);	
   	return supermax;
}







