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

#include "tc4.h"
#include "util.h"

NBC4::NBC4(const PatentList &patents) : patents{patents} {
	calculateClasses();
	calculateSections();
	
	calculatePriors();
	
	calculateGaussians();
}

void NBC4::calculateClasses(){
	std2::for_each(patents, [this](const Patent &p){
		for(SubClass sc : p.subclasses){
			classes.insert(sc.class_);
		}
	});
}

void NBC4::calculateSections(){
	std2::for_each(patents, [this](const Patent &p){
		for(SubClass sc : p.subclasses){
			sections.insert(sc.class_.section);
		}
	});
}

void NBC4::calculatePriors(){
	priorsClasses = calculatePriors(classes);
	priorsSections = calculatePriors(sections);
}

template <typename T>
std::unordered_map<T, double> NBC4::calculatePriors(const std::unordered_set<T> &collection){
	std::unordered_map<T, unsigned long> counts;
	
	for(const auto& e: collection){
		for(const auto &p : patents){
			if(p.subclasses.front() < e){
				++counts[e];
			}
		}
	}
	
	auto sum = std2::accumulate(counts, 0, [](unsigned long sum, const std::pair<T, unsigned long> &p){return sum+p.second;});
	
	std::unordered_map<T, double> priors;
	foreach(counts, t, count){
		priors[t] = double(count)/sum;
	});
	return priors;
}

void NBC4::calculateGaussians(){
	gaussiansClasses = calculateGaussians(classes);
	gaussiansSections = calculateGaussians(sections);
}

template <typename T>
std::unordered_map<T, std::unordered_map<term, gaussian>> NBC4::calculateGaussians(const std::unordered_set<T> &collection){
	std::unordered_map<T, std::unordered_map<term, gaussian>> gaussians;
	for(const auto& e : collection){
		std::unordered_map<term, std::vector<frequency>> counts;
		for(const auto &p : patents){
			if(p.subclasses.front() < e){
				foreach(p.terms, term, const frequency){
					counts[term].push_back(frequency);
				});
			}
		}
		
		foreach(counts, term, frequencies){
			const auto m = mean(frequencies);
			const auto var = variance(frequencies, m);
			
			gaussians[e].insert({term, {m, var}});
		});
	}
	return gaussians;
}

void NBC4::classify(Patent &p){
	std::unordered_map<Class, double> scores;
	
	for(const auto& c : classes){
		scores[c] = std::log(priorsClasses[c]);
		
		scores[c] += std::log(priorsSections[c.section]);
		
		foreach(p.terms, term, const f){
			if(f > threshold){
				double modifier = f/double(threshold);
				scores[c] += modifier*std::log(gaussiansClasses[c][term](f));
				scores[c] += modifier*std::log(gaussiansSections[c.section][term](f));
			}
		});
	}
	
	auto best1 = maxValue(scores);
	scores.erase(best1.first);
	auto best2 = maxValue(scores);
	scores.erase(best2.first);
	auto best3 = maxValue(scores);
	
	p.predictions.push_back(best1.first);
	p.predictions.push_back(best2.first);
	p.predictions.push_back(best3.first);
}