.POSIX:
.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
CXXFLAGS = -std=c++11
LDLIBS = -pthread

.cpp.o:
	$(CXX) $(CXXFLAGS) $(LDLIBS) -c $<

BIN = test-2 test-3 test-5
OBJ = ConcurrentHashMap.cpp

all: $(BIN) $(OBJ)

compilar: 
	$(CXX) $(CXXFLAGS) $(LDLIBS) ConcurrentHashMap.h
	$(CXX) $(CXXFLAGS) $(LDLIBS) CountWord.h
	$(CXX) $(CXXFLAGS) $(LDLIBS) -g -o test test_addMaxi.cpp ConcurrentHashMap.cpp
	$(CXX) $(CXXFLAGS) $(LDLIBS) -g -o test2 test_countWords.cpp ConcurrentHashMap.cpp
	$(CXX) $(CXXFLAGS) $(LDLIBS) -g -o test3 test_maximum.cpp ConcurrentHashMap.cpp
	$(CXX) $(CXXFLAGS) $(LDLIBS) -g -o test4 test_exmutua.cpp ConcurrentHashMap.cpp

ejecutar: test test2 test3 test4
	./test
	./test2
	./test3
	./test4

$(BIN): ListaAtomica.hpp

test-2: $(OBJ) test-2.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-2.cpp $(OBJ) $(LDLIBS)
	
test-2-run: test-2
	awk -f corpus.awk corpus | sort >corpus-post
	./test-2 | sort | diff -u - corpus-post
	rm -f corpus-post

test-3: $(OBJ) test-3.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-3.cpp $(OBJ) $(LDLIBS)

test-3-run: test-3
	awk -f corpus.awk corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-3 $$((i + 1)) | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]

test-4: $(OBJ) test-4.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-4.cpp $(OBJ) $(LDLIBS)

test-4-run: test-4
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	cat corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-4 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]


test-5: $(OBJ) test-5.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-5.cpp $(OBJ) $(LDLIBS)

test-5-run: test-5
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	cat corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-5 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]

clean:
	rm -f test
	rm -f test2
	rm -f test3
	rm -f test4
	rm -f test-2
	rm -f test-3
	rm -f test-4
	rm -f test-5
	rm -f corpus-*
