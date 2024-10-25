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
#ifndef HEADER_FIXEDLISIT
#define HEADER_FIXEDLISIT

namespace DMUtils {

    template <typename T>
    class FixedSizeList {
        public:
        FixedSizeList();//no maximum size ; requires a later call to init
        FixedSizeList(size_t size);//specifies maximum size
        ~FixedSizeList();
        void init(size_t size);//inits the list (supposes list hasn't been used before)

        size_t size(void) const;//returns current list's size
        size_t max_size(void) const;//return the maximum number of T the list can hold
        bool isEmpty(void) const;//true if the list is empty
        bool isFull(void) const;//true if the list if full

        T pop_back(void);
        T pop_front(void);
        void push_back(T t);
        void push_front(T t);
        T begin(void) const;
        T end(void) const;
        T get(size_t i);

        T operator[](size_t i);

        private:
        T* _data;
        size_t _first, _last;//respectively indice of the beginning and of the end of the list, except when _stored=0
        size_t _max_size;
        size_t _stored;//size stored
    };
}

#include "FixedSizeList.tpl"

#endif // HEADER_FIXEDLISIT
