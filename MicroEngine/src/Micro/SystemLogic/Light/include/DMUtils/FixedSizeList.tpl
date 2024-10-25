/**
BSD 2-Clause License

Copyright (c) 2017, DaiMysha
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "FixedSizeList.h"

#include <assert.h>

namespace DMUtils {

	template <typename T>
    FixedSizeList<T>::FixedSizeList() {
        _data=nullptr;
        init(0);
    }

    template <typename T>
    FixedSizeList<T>::FixedSizeList(size_t size) {
        init(size);
    }

    template <typename T>
    FixedSizeList<T>::~FixedSizeList() {
        delete[]_data;
    }

    template <typename T>
    void FixedSizeList<T>::init(size_t size) {
        if(size) _data = new T[size];
        else _data = 0;
        _first = _last = 0;
        _max_size = size;
        _stored = 0;
    }

    template <typename T>
    size_t FixedSizeList<T>::size(void) const {
        return _stored;
    }

    template <typename T>
    size_t FixedSizeList<T>::max_size(void) const {
        return _max_size;
    }

    template <typename T>
    bool FixedSizeList<T>::isEmpty(void) const {
        return _stored==0;
    }

    template <typename T>
    bool FixedSizeList<T>::isFull(void) const {
        return _stored==_max_size;
    }

    template <typename T>
    T FixedSizeList<T>::pop_back(void) {
        assert(!isEmpty());
        T tmp=_data[_last];
        if(_last==0) _last=_max_size-1;
        else --_last;
        if(_stored>0) --_stored;
        return tmp;
    }

    template <typename T>
    T FixedSizeList<T>::pop_front(void) {
        assert(!isEmpty());
        T tmp = _data[_first++];
        if(_first==_max_size) _first=0;
        if(_stored>0) --_stored;
        return tmp;
    }

    template <typename T>
    void FixedSizeList<T>::push_back(T t) {
        if(isEmpty()) {
            _first = _last = 0;
            _data[_first]=t;
            _stored=1;
            return;
        } else if(isFull()) {
            if(++_first==_max_size) _first=0;
        }
        if(++_last==_max_size) _last=0;
        _data[_last]=t;
        if(_stored<_max_size) ++_stored;
    }

    template <typename T>
    void FixedSizeList<T>::push_front(T t) {
        if(isEmpty()) {
            _first = _last = 0;
            _data[_first]=t;
            _stored=1;
            return;
        } else if(isFull()) {
            if(_last==0) _last=_max_size-1;
        }
        if(_first==0) _first=_max_size-1;
        else --_first;
        _data[_first]=t;
        if(_stored<_max_size) ++_stored;
    }

    template <typename T>
    T FixedSizeList<T>::begin(void) const {
        assert(!isEmpty());
        return _data[_first];
    }

    template <typename T>
    T FixedSizeList<T>::end(void) const {
        assert(!isEmpty());
        return _data[_last];
    }

    template <typename T>
    T FixedSizeList<T>::get(size_t i) {
        assert(i>=0&&i<_max_size);
        assert(!isEmpty());
        size_t pos = _first+i;
        while(pos>=_max_size) pos-=_max_size;
        return _data[pos];
    }
	
	template <typename T>
	T operator[](size_t i) {
		return get(i);
	}
}
