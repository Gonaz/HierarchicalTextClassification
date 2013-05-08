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

#include "tc7.h"
#include "util.h"

NBC7::NBC7(const PatentList &patents) : patents{patents} {
	calculateClasses();
	calculateSections();
	
	calculatePriors();
	
	calculateGaussians();
	calculateLocals();
}

void NBC7::calculateLocals(){
	for(const auto& s : sections){
		std::unordered_set<Class> cs;
		for(const auto& p : patents){
			for(const auto& sc : p.subclasses){
				if(sc < s){
					cs.insert(sc.class_);
				}
			}
		}
		
		priorsClassesFromSection[s] = calculatePriors(cs);
		gaussiansClassesFromSection[s] = calculateGaussians(cs);
	}
}

void NBC7::calculateClasses(){
	std2::for_each(patents, [this](const Patent &p){
		for(SubClass sc : p.subclasses){
			classes.insert(sc.class_);
		}
	});
}

void NBC7::calculateSections(){
	std2::for_each(patents, [this](const Patent &p){
		for(SubClass sc : p.subclasses){
			sections.insert(sc.class_.section);
		}
	});
}

void NBC7::calculatePriors(){
	priorsSections = calculatePriors(sections);
}

template <typename T>
std::unordered_map<T, double> NBC7::calculatePriors(const std::unordered_set<T> &collection){
	std::unordered_map<T, unsigned long> counts;
	
	for(const auto& e: collection){
		for(const auto &p : patents){
			for(const auto& cs : p.subclasses){
				if(cs < e){
					++counts[e];
				}
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

void NBC7::calculateGaussians(){
	gaussiansSections = calculateGaussians(sections);
}

template <typename T>
std::unordered_map<T, std::unordered_map<term, gaussian>> NBC7::calculateGaussians(const std::unordered_set<T> &collection){
	std::unordered_map<T, std::unordered_map<term, gaussian>> gaussians;
	for(const auto& e : collection){
		std::unordered_map<term, std::vector<frequency>> counts;
		for(const auto &p : patents){
			for(const auto& sc : p.subclasses){
				if(sc < e){
					foreach(p.terms, term, frequency){
						counts[term].push_back(frequency);
					});
				}
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

void NBC7::classify(Patent &p){
	std::unordered_map<Section, double> scoresSections;
	
	for(const auto& s : sections){
		scoresSections[s] = std::log(priorsSections[s]);
		
		foreach(p.terms, term, const f){
			if(f > threshold){
				double modifier = f/double(threshold);
				scoresSections[s] += modifier*std::log(gaussiansSections[s][term](f));
			}
		});
	}
	
	auto bestSection1 = maxValue(scoresSections);
	scoresSections.erase(bestSection1.first);
	auto bestSection2 = maxValue(scoresSections);
	scoresSections.erase(bestSection2.first);
	auto bestSection3 = maxValue(scoresSections);
	
	const std::vector<Section> bestSections = {bestSection1.first, bestSection2.first, bestSection3.first};
	std::unordered_map<Class, double> scoresClasses;
	
	for(const auto& section : bestSections){
		for(const auto& c : classes){
			if(c < section){
				scoresClasses[c] = std::log(priorsSections[section]) + std::log(priorsClassesFromSection[section][c]);
			
				foreach(p.terms, term, const f){
					if(f > threshold){
						double modifier = f/double(threshold);
						scoresClasses[c] += modifier*std::log(gaussiansClassesFromSection[section][c][term](f));
					} 
				});
			}
		}
	}

	auto bestClass1 = maxValue(scoresClasses);
	scoresClasses.erase(bestClass1.first);
	auto bestClass2 = maxValue(scoresClasses);
	scoresClasses.erase(bestClass2.first);
	auto bestClass3 = maxValue(scoresClasses);
	
	p.predictions.push_back(bestClass1.first);
	p.predictions.push_back(bestClass2.first);
	p.predictions.push_back(bestClass3.first);
}