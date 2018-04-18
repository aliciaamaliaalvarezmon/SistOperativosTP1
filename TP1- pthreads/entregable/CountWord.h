#ifndef COUNT_WORD_H__
#define COUNT_WORD_H__

#include "ConcurrentHashMap.h"

ConcurrentHashMap count_words(string arch);
ConcurrentHashMap count_words2(list<string>archs);
ConcurrentHashMap count_words3(unsigned int n, list<string>archs);
pair<string, unsigned int> maximum(unsigned int p_archivos, unsigned int p_maximos, list<string>archs);



#endif