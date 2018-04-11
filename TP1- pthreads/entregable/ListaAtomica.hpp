#ifndef LISTA_ATOMICA_H__
#define LISTA_ATOMICA_H__

#include <atomic>

#define nullptr 0

template <typename T>
class Lista {
private:
	struct Nodo {
		Nodo(const T& val) : _val(val), _next(nullptr) {}
		T _val;
		Nodo *_next;
	};

	std::atomic<Nodo *> _head;

public:
	Lista() : _head(nullptr) {}
	~Lista() { 
		Nodo *n, *t;
		n = _head.load();
		while (n) {
			t = n;
			n = n->_next;
			delete t;
		}
	}

	/*void push_front(const T& val) {//Asumo que esta operacion es para agregar nodos a la lista				
		if( _head.load() == nullptr){			
			_head.store(new Nodo(val));			
		}else{
			std::atomic<Nodo *> buscador;
			buscador = _head.load();
			while(buscador.load()->_next != nullptr){
				buscador = buscador.load()->_next;
			}								
			buscador.load()->_next = new Nodo(val);					
		}
		
	}*/

/*	void push_front(const T & val){
		std::atomic<Nodo *> aux;
		std::atomic<Nodo > nuevo;
		aux.store(_head.load());		
		nuevo.store(new Nodo(val));
		_head.store(nuevo);
		nuevo->_next.store(aux);

	}
*/
	void push_front(const T & val){		
 		std::atomic<Nodo*> nuevo;
 		nuevo.store(new Nodo(val)); 		
 		while(true){
 			Nodo* aux = _head;	
 			if(_head.compare_exchange_strong(aux, nuevo)){
 				nuevo->next = aux;
 				break;
 			}
 		}
	}

		/* Completar. Debe ser atómico. */
	



	T& front() const {
		return _head.load()->_val;
	}

	T& iesimo(int i) const {
		Nodo *n = _head.load();
		int j;
		for (int j = 0; j < i; j++)
			n = n->_next;
		return n->_val;
	}

	class Iterador {//Es un Iterador estático. Si se modifica la lista hay que crear Iterador nuevo.
	public:
		Iterador() : _lista(nullptr), _nodo_sig(nullptr) {}

		Iterador& operator = (const typename Lista::Iterador& otro) {
			_lista = otro._lista;
			_nodo_sig = otro._nodo_sig;
			return *this;
		}

		bool HaySiguiente() const {
			return _nodo_sig != nullptr;
		}

		T& Siguiente() {
			return _nodo_sig->_val;
		}



		void Avanzar() {
			_nodo_sig = _nodo_sig->_next;
		}

		bool operator == (const typename Lista::Iterador& otro) const {
			return _lista._head.load() == otro._lista._head.load() && _nodo_sig == otro._nodo_sig;
		}

	private:
		Lista *_lista;

		typename Lista::Nodo *_nodo_sig;

		Iterador(Lista<T>* lista,typename Lista<T>::Nodo* sig) : _lista(lista), _nodo_sig(sig) {}
		friend typename Lista<T>::Iterador Lista<T>::CrearIt();
	};

	Iterador CrearIt() {
		return Iterador(this, _head);
	}
};

#endif /* LISTA_ATOMICA_H__ */










/*

	void push_front(const T& val) {//Asumo que esta operacion es para agregar nodos a la lista
		if( _head.load() == nullptr){
			_head.load()->_val = val;
		}else{
			Iterador it = CrearIt();
			while(HaySiguiente(it)){
				it.Avanzar();
			}
			it.Snew(atomic<Nodo >);




		}

		/* Completar. Debe ser atómico. */
/*	}*/