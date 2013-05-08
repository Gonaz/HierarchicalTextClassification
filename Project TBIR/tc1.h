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

#ifndef __Project_TBIR__tc1__
#define __Project_TBIR__tc1__

#include "class.h"
#include "gaussian.h"
#include "patent.h"
#include <unordered_set>
#include <unordered_map>

struct NBC1 {
	NBC1(const PatentList &patents);
	void classify(Patent &p);
private:
	const PatentList &patents;
	std::unordered_set<Class> classes;
	
	std::unordered_map<Class, double> priorsClasses;
	std::unordered_map<Class, std::unordered_map<term, gaussian>> gaussiansClasses;

	void calculateClasses();
	
	template <typename T>
	std::unordered_map<T, double> calculatePriors(const std::unordered_set<T> &collection);
	void calculatePriors();
	
	template <typename T>
	std::unordered_map<T, std::unordered_map<term, gaussian>> calculateGaussians(const std::unordered_set<T> &collection);
	void calculateGaussians();
};

typedef NBC1 NBC;

#endif /* defined(__Project_TBIR__tc1__) */
