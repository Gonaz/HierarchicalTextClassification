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

#include "io.h"
#include "std2.h"
#include "util.h"
#include <unordered_map>
#include <fstream>
#include <sstream>

PatentList IO::readPatents(const std::string &filename){
	std::ifstream file(filename);
	std::deque<std::string> lines;
	
	if(file.is_open()){
		std::string line;
		while(file.good()){
			std::getline(file, line);
			lines.push_back(line);
		}
	}
	file.close();
	
	PatentList patents;
	
	std::deque<std::string> info;
	std::vector<std::string> cats;
	std::unordered_map<term, frequency> info_;
	std::string str;
	
	for(const auto& line : lines){
		if(line.size() == 0){
			break;
		}
		std::istringstream iLine(line);
		
		while (std::getline(iLine, str, ' ')) {
			info.push_back(str);
		}
		
		std::istringstream iCat(info.front());
		while(std::getline(iCat, str, ',')){
			cats.push_back(str);
		}
		info.erase(std::begin(info));

		info_.reserve(info.size());
		for(auto i : info){
			const auto pos = i.find(':');
			info_.insert({std::stoul(i.substr(0, pos)), std::stoul(i.substr(pos+1))});
		}
		
		patents.push_back(Patent::create(cats, info_));
		
		info.clear();
		cats.clear();
		info_.clear();
	}
	
	return patents;
}

void IO::writePatents(const PatentList &patents, const std::string &filename){
	std::ofstream file{filename};
	
	for(auto it=std::begin(patents); it!=std::end(patents); ++it){
		auto patent = *it;
		for(const auto& prediction : patent.predictions){
			file << prediction << " ";
		}
		
		if(it != std::end(patents)-1){
			file << "\n";
		}
	}
	
	file.close();
}