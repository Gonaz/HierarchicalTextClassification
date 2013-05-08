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

#ifndef Project_TBIR_util_h
#define Project_TBIR_util_h

#include "std2.h"
#include "class.h"
#include <unordered_map>
#include <chrono>
#include <ostream>

#define foreach(c, k, v) foreach_(c, [&](const typename std::remove_reference<decltype(c)>::type::key_type &k, typename std::remove_reference<decltype(c)>::type::mapped_type v)

template <typename T, typename F>
void foreach_(T &c, F f){
    for(auto &kv : c){
        f(kv.first, kv.second);
    }
}

inline
std::ostream& operator<<(std::ostream& os, const Class& c){
	return os << c.name;
}

template <typename T>
double mean(const std::vector<T> &v){
	return double(std2::accumulate(v, T(0)))/v.size();
}

template <typename T>
double variance(const std::vector<T> &v, const double mean){
    return std::inner_product(std::begin(v), std::end(v), std::begin(v), 0.0, std::plus<double>(), [mean](const T p, const T e){return p*(e-mean);}) / v.size();
}

template <typename K>
std::pair<K, double> maxValue(const std::unordered_map<K, double> &scores){
	double bestScore = -std::numeric_limits<double>::infinity();
	std::string name;
	foreach(scores, e, const score){
		if(score >= bestScore){
			bestScore = score;
			name = e.name;
		}
	});
	
	return {name, bestScore};
}

typedef std::chrono::time_point<std::chrono::system_clock> timepoint;

inline
timepoint now(){
	return std::chrono::system_clock::now();
}

inline
size_t diff(const timepoint &start, const timepoint &end){
	return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

#endif
