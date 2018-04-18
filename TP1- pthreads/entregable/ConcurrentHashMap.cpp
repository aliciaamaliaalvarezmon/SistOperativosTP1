#include "ConcurrentHashMap.h"
#include "CountWord.h"
//template <typename T>



using namespace std;

//std::atomic<int> maxi(0);
//std::atomic<int> addi(0);
//int nada = 0;

//mutex posicion[26];
mutex m;
mutex m2;
mutex posicion[26];
mutex max_mut;
pair<string, int> _maximo("", 0);
mutex ej4;
mutex ej4b;
mutex ej5;
mutex ej5b;
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
//	while(!maxi.compare_exchange_weak(nada, maxi)){
//	}
//	addi.fetch_add(1);
	int pos = hash_func(key);	
	//cout << "entro un add" << " clave: " << key << endl;
	posicion[pos].lock();
	//cout << "paso un add" << " clave: " << key << endl;
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
	//cout << "va a salir un add" << " clave: " << key << endl;
	posicion[pos].unlock();
//	addi.fetch_sub(1); 
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

void * ConcurrentHashMap::maxaux(atomic<int> &ultima){	
		int x;
		while((x = ultima.fetch_add(1)) < 26){
			Lista<pair<string, int>>::Iterador it =(*tabla[x]).CrearIt();												
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


void* max(void* aux){			
	Hashcontador * caux = (Hashcontador*) aux;
	//ConcurrentHashMap * conchashmap = static_cast<ConcurrentHashMap*>(aux);
	(caux->h)->maxaux(caux->_ultima);
	return nullptr;
}

pair<string, int> ConcurrentHashMap::maximum(unsigned int nt){	
	//maxi.fetch_add(1);
	//while(!addi.compare_exchange_weak(nada, addi)){
//	}
	//cout << "entra un max" << endl;
	for(int i = 0; i < 26; i++){
		posicion[i].lock();
	}
	//cout << "paso un max un max" << endl;
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
		pthread_create(&thread[tid], NULL, max, &aux );//le pasa a max el struct Hashcontador, con nuestro hash y la thread		
	}
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	}
  // 	maxi.fetch_sub(1); 
  	for(int i = 0; i < 26; i++){
		posicion[i].unlock();
	}
	//cout << "salio un max" << endl;  	 	
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
		(caux->h)->addAndInc(alo); 
	}		
	return nullptr;	
	
}

//version vieje

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






//cambiar estos locks version vieja aux
/*void * count_words_limthreads_aux(void* aux){
	HashescritorConc * caux = (HashescritorConc*) aux;		
	while((caux->ite) != ((caux->lista)->end())){	
		ej4.lock();
		const char* archivo = (*(caux)->ite).c_str();
		ej4.unlock();
		ifstream input;
		input.open(archivo);
		string alo;
		while(!input.eof()){
			input >> alo;
			(caux->h)->addAndInc(alo); 
		}
		ej4b.lock();			
		(caux->ite)++;
		ej4b.unlock();			
	}	
			
	return nullptr;	
	
}



//version vieja
ConcurrentHashMap count_words3(unsigned int n, list<string>archs){
	int realnt;
	if(n <= archs.size()){
		realnt = n;
	}else{
		realnt = archs.size();
	}	
	ConcurrentHashMap escri;		
	HashescritorConc* separador = new (HashescritorConc);
	separador->h = &escri;
	separador->lista = &archs;
	separador->ite =  archs.begin();//iterador a principio de lista			
	pthread_t thread[realnt];
	int tid;
	for(tid = 0; tid <  realnt; tid++  ){	
		pthread_create(&thread[tid], NULL, count_words_limthreads_aux, separador);//le pasa a max el struct Hashcontador, con nuestro hash y la thread	
		
	}		
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	}  
   	delete(separador); 
   	return escri;

	
}*/


//version nueva aux
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
			(caux->h)->addAndInc(alo);
		}		
		input.close();		
	}			
	return nullptr;	
	
}

//version nueva
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

/*
void * count_words_limthreads_aux2(void* aux){	
	HashescritorConc2 * caux = (HashescritorConc2*) aux;
	while((caux->ite) != ((caux->lista)->end())){ //and ((*(caux->ult_escri)) < (caux->h)->size())){
	ej5.lock();				
		const char* archivo = (*(caux)->ite).c_str();		
		ConcurrentHashMap* hash_actual = (*(caux->h))[(*(caux->ult_escri))];
		(*(caux->ult_escri))++;
		(caux->ite)++;		
		(*hash_actual) = count_words(archivo);	
	ej5.unlock();		
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
	list<string>::iterator	it= std::begin(archs);
	int _ultima = 0;
	//HashescritorConc2* separador[realnt];
	HashescritorConc2* separador = new (HashescritorConc2);
	separador->h = &escri;
	separador->lista = &archs;
	separador->ite = it;
	separador->ult_escri = &_ultima;	
	pthread_t thread[realnt];
	int tid;
	for(tid = 0; tid < realnt; tid++){
		pthread_create(&thread[tid], NULL, count_words_limthreads_aux2, separador);
	}
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	} 
   	//aqui todos los hashmap de escri estan llenos    
   ConcurrentHashMap hash_recolector;
   for(int i =0; i < escri.size(); i++){
   	for(int j = 0; j< 26; j++){
   		Lista<pair< string, int> >::Iterador it = (escri[i]->entrada(j))->CrearIt();
   		while(it.HaySiguiente()){
   			hash_recolector.addAndInc(it.Siguiente().first);
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


*/


void * count_words_limthreads_aux2(void* aux){	
	HashescritorConc2 * caux = (HashescritorConc2*) aux;
	int x;
	while( (x = (caux->ult_pos).fetch_add(1)) < ((caux->vecti)->size())){//and ((*(caux->ult_escri)) < (caux->h)->size())){		
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
	//int _ultima = 0;	
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
   	//aqui todos los hashmap de escri estan llenos    
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
   	//cout <<"es el maximum"<< endl;
   	pair<string, int> supermax = hash_recolector.maximum(p_maximos);
   	//cout <<" no es el maximum"<< endl;   	
  	for (int i = 0; i < archs.size(); ++i){       
        delete(escri[i]);
   	}
   	return supermax;
}



pair<string, unsigned int> maximumConcurrente(unsigned int p_archivos, unsigned int p_maximos, list<string>archs){	
	int realnt;
	if(p_archivos <= archs.size()){
		realnt = p_archivos;
	}else{
		realnt = archs.size();
	}		
	ConcurrentHashMap  escri;		
   	vector<string> archivador;
	list<string>::iterator	iteaux = archs.begin();
	while(iteaux != archs.end()){
		archivador.push_back((*iteaux));
		iteaux++;
	}   
	//aqui archivador tiene la lista archs
	//int _ultima = 0;	
	HashescritorConc* separador = new (HashescritorConc);
	separador->h = &escri;
	separador->vecti = &archivador;
	separador->ult.store(0);//ultimo archivo abierto	
	pthread_t thread[realnt];
	int tid;
	for(tid = 0; tid < realnt; tid++){
		pthread_create(&thread[tid], NULL, count_words_limthreads_aux, separador);
	}
	for (tid = 0; tid < realnt; ++tid){
        pthread_join(thread[tid], NULL);
   	}
   	//cout <<"es el maximum"<< endl;
   	pair<string, int> supermax = escri.maximum(p_maximos);
   	//cout <<" no es el maximum"<< endl;
   	delete(separador);      	
   	return supermax;
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

