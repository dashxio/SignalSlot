#ifndef RECEIVER_IMPL_CPP
#define RECEIVER_IMPL_CPP

#include "../include/receiver.h"

template<typename T>
void Receiver<T>::setValue(T t){
    m_value = t;
}

#endif
