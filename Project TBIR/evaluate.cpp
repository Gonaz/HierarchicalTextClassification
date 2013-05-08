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

#include "evaluate.h"
#include "exception.h"
#include <algorithm>
#include <map>
#include <stdexcept>

Evaluate::Evaluate(std::string s){
	std2::transform(s, s, [](const char &c){return std::tolower(c);});
	
	std::function<bool(const Patent &p)> tp = std::bind(&Evaluate::topPrediction, this, std::placeholders::_1);
	std::function<bool(const Patent &p)> tg = std::bind(&Evaluate::threeGuesses, this, std::placeholders::_1);
	std::function<bool(const Patent &p)> ac = std::bind(&Evaluate::allCategories, this, std::placeholders::_1);

	std::map<std::string, std::function<bool(const Patent &p)>> evalFunction;
	evalFunction.insert({{"topprediction", tp}, {"top prediction", tp}});
	evalFunction.insert({{"threeguesses", tg}, {"three guesses", tg}});
	evalFunction.insert({{"allcategories", ac}, {"all categories", ac}});
	
	try {
		eval = evalFunction.at(s);
	} catch (const std::out_of_range &e) {
		throwException("Unknown evaluation function");
	}
}

bool Evaluate::operator()(const Patent &p) const{
	if(p.predictions.size() != 3){
		throwException("Number of predictions should be 3");
	}
	return eval(p);
}

size_t Evaluate::operator()(const PatentList &v) const{
	return std2::accumulate(v, 0, [this](size_t sum, const Patent &p){return operator()(p) ? sum+1 : sum;});
}

bool Evaluate::topPrediction(const Patent &p) const{
	return p.subclasses.front() < p.predictions.front();
}

bool Evaluate::threeGuesses(const Patent &p) const{
	return std2::any_of(p.predictions, [&p](const Class &c){return p.subclasses.front() < c;});
}

bool Evaluate::allCategories(const Patent &p) const{
	return std2::any_of(p.subclasses, [&p](const SubClass &sc){return sc < p.predictions.front();});
}