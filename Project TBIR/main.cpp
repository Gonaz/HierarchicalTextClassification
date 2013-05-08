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

#include <iostream>
#include "std2.h"
#include "test.h"
#include "evaluate.h"
#include "io.h"
#include "util.h"

//#include "tc1.h"
//#include "tc2.h"
//#include "tc3.h"
//#include "tc4.h"
#include "tc5.h"
//#include "tc6.h"
//#include "tc7.h"

#include <string>
#include <chrono>
#include <thread>
#include <map>

template <typename NBC>
void classify(NBC nbc, PatentList &patents){
	for(auto &p : patents){
		nbc.classify(p);
	}
}

int main(){
	runTests();
//	return 0;

	const std::string &path{"/Users/Jonas/Dropbox/KUL/Text Based Information Retrieval/Project/data/"};
    const std::string &trainFilename{path + "wipoalpha-train.txt"};
	const std::string &testFilename{path + "wipoalpha-test.txt"};
	const std::string &outputFilename{path + "result.txt"};
	
	timepoint startReading, endReading, startTraining, endTraining, startTesting, endTesting, startWriting, endWriting;
	Evaluate e;
	IO io;
	
	startReading = now();
	PatentList db {io.readPatents(trainFilename)};
	PatentList patents{io.readPatents(testFilename)};
	unsigned int nbThreads = std::thread::hardware_concurrency();
//	nbThreads = 1;
	
	size_t threadSize = patents.size()/nbThreads;
	std::map<unsigned int, PatentList> ps;
	size_t offset = 0;
	for(unsigned int t=0; t<nbThreads-1; ++t){
		std::copy_n(std::begin(patents) + offset, threadSize, std::back_inserter(ps[t]));
		offset += threadSize;
	}
	std::copy(std::begin(patents)+offset, std::end(patents), std::back_inserter(ps[nbThreads-1]));
	patents.clear();
	endReading = now();
	
	startTraining = now();
	NBC nbc = db;
	endTraining = now();
	
	startTesting = now();
	std::vector<std::thread> threads;
	for(unsigned int t=0; t<nbThreads; ++t){
		threads.push_back(std::thread([&nbc, &ps, t](){classify(nbc, ps[t]);}));
	}
	
	for(auto &t : threads){
		t.join();
	}
	
	for(unsigned int t=0; t<nbThreads; ++t){
		for(auto &p : ps[t]){
			patents.push_back(p);
		}
		ps[t].clear();
	}
	endTesting = now();
	
	std::cout << "Correct " << e(patents) << "/" << patents.size() << std::endl;
	
	Evaluate e2("three guesses");
	Evaluate e3("all categories");
	
	std::cout << std::endl;
	std::cout << "Top Prediction " << double(e(patents))/patents.size()*100 << std::endl;
	std::cout << "Three Guesses " << double(e2(patents))/patents.size()*100 << std::endl;
	std::cout << "All Categories " << double(e3(patents))/patents.size()*100 << std::endl;
	std::cout << std::endl;
	
	io.writePatents(patents, outputFilename);

	std::cout << "Reading time " << diff(startReading, endReading) << std::endl;
	std::cout << "Training time " << diff(startTraining, endTraining) << std::endl;
	std::cout << "Testing time " << diff(startTesting, endTesting) << std::endl;

	std::cout << "Finished :)" << std::endl;
}