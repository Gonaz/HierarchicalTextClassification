// Copyright (c) 2013, Jonas Vanthornhout
// All rights reserved.
 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef Project_TBIR_std2_h
#define Project_TBIR_std2_h

#include <cmath>
#include <algorithm>
#include <numeric>

namespace std2 {
	template <typename T1, typename T2, typename F>
    typename T2::iterator transform(T1 &v1, T2 &v2, F f) {
        return std::transform(std::begin(v1), std::end(v1), std::begin(v2), f);
    }
	
	template <typename C, typename K>
	bool contains(C c, K k){
		return c.count(k) > 0;
	}
	
	template <typename T1, typename T2>
    T2 accumulate(T1 v, T2 init){
        return std::accumulate(std::begin(v), std::end(v), init);
    }
    
    template <typename T1, typename T2, typename F>
    T2 accumulate(T1 v, T2 init, F f){
        return std::accumulate(std::begin(v), std::end(v), init, f);
    }
	
	template <typename T, typename F>
	bool any_of(T t, F f){
		return std::any_of(std::begin(t), std::end(t), f);
	}
	
	template <typename T, typename F>
    F for_each(const T &v, F f){
        return std::for_each(std::begin(v), std::end(v), f);
    }
	
	const double pi = std::atan(1) * 4;
}

#endif
