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

#ifndef Project_TBIR_test_h
#define Project_TBIR_test_h

#include "subclass.h"
#include "patent.h"
#include "evaluate.h"
#include "exception.h"
#include "section.h"
#include <iostream>
#include <functional>

#define assertEquals(expected, actual) {auto a=actual; if(a != expected){std::stringstream msg; msg << "expected: " << expected << ", but got " << a; std::stringstream location; location << __FUNCTION__ << "(" << __LINE__ << ")";throw exception{location.str(), msg.str()};}}
#define assertTrue(actual) {auto a=actual; if(!a){std::stringstream msg; msg << "expected that " << #actual << " should be true, but it was false"; std::stringstream location; location << __FUNCTION__ << "(" << __LINE__ << ")";throw exception{location.str(), msg.str()};}}
#define assertFalse(actual) {auto a=actual; if(a){std::stringstream msg; msg << "expected that " << #actual << " should be false, but it was true"; std::stringstream location; location << __FUNCTION__ << "(" << __LINE__ << ")";throw exception{location.str(), msg.str()};}}

inline
std::string operator "" _S(const char* s, size_t n){
	return std::string{s, n};
}

void test_SubClass_constructor(){
	std::string name{"a01b"};
	SubClass sc{name};
	assertEquals(name, sc.name);
}

void test_SubClass_neq(){
	SubClass sc1{"a01b"};
	SubClass sc3{"a01c"};
	
	if(sc1 != sc3){
	} else {
		throwException("subclasses with a different name should be different");
	}
}

void test_SubClass_eq(){
	SubClass sc1{"a01b"};
	SubClass sc3{"a01c"};
	
	if(sc1 == sc3){
		throwException("subclasses with a different name should be different");
	}
}

void test_SubClass_lt(){
	SubClass sc1{"a01b"};
	Class c1{"a01"};
	Class c2{"a02"};
	
	assertTrue(sc1 < c1);
	assertFalse(sc1 < c2);
}

void test_SubClass_class(){
	SubClass sc1{"a01a"};
	SubClass sc2{"a01b"};
	if(sc1.class_ != sc2.class_){
		throwException("even if the subclass name is different, the classes can be the same");
	}
	
	SubClass sc3{"a02a"};
	if(sc1.class_ == sc3.class_){
		throwException("different names for classes should make the classes different");
	}
}

void test_Class_constructor(){
	std::string name = "a01";
	Class c{name};
	assertEquals(name, c.name);
}

void test_Class_neq(){
	Class c1{"a01"};
	Class c3{"a02"};
	
	if(c1 != c3){
	} else {
		throwException("classes with different names should be different");
	}
}

void test_Class_eq(){
	Class c1{"a01"};
	Class c3{"b01"};
	
	if(c1 == c3){
		throwException("classes with a different name should be different");
	}
}

void test_Class_section(){
	Class c1{"a01"};
	Class c2{"a02"};
	if(c1.section != c2.section){
		throwException("even if the class name is different, the sections can be the same");
	}
	
	Class sc3{"b01"};
	if(c1.section == sc3.section){
		throwException("different names for sections should make the sections different");
	}
}

void test_Section_constructor(){
	std::string name = "a";
	Section s{name};
	assertEquals(name, s.name);
}

void test_Section_neq(){
	Section s1{"a"};
	Section s3{"b"};
	
	if(s1 != s3){
	} else {
		throwException("sections with different names should be different");
	}
}

void test_Section_eq(){
	Section s1{"a"};
	Section s3{"b"};
	
	if(s1 == s3){
		throwException("sections with a different name should be different");
	}
}

void test_Patent_constructor(){
	std::vector<SubClass> scs1 = {"a01a"_S, "a01b"_S};
	std::vector<std::string> scs1_name;
	std::transform(std::begin(scs1), std::end(scs1), std::back_inserter(scs1_name), [](const SubClass &sc){return sc.name;});
	
	std::unordered_map<term, frequency> terms {{1, 10}};
	Patent p1 = Patent::create(scs1_name, terms);
	if(p1.subclasses.at(0) != scs1.at(0)){
		throwException("the subclass of a patent should be equal to the given subclass");
	}
	
	if(p1.subclasses.at(1) != scs1.at(1)){
		throwException("the subclass of a patent should be equal to the given subclass");
	}
	
	if(p1.terms.at(1) != 10){
		throwException("the frequency of a term should be equal to the given frequency");
	}
	
	if(p1.subclasses.size() != scs1.size()){
		throwException("the number of subclasses doesn't match the number of given subclasses");
	}
	
	if(p1.terms.size() != terms.size()){
		throwException("the number of terms doesn't match the number of given terms");
	}
	
	if(p1.predictions.size() != 0){
		throwException("the number of predictions should initially be zero");
	}
}

void test_topPrediction(const Evaluate &e){
	std::vector<SubClass> scs1 = {"a01a"_S, "a01b"_S};
	std::vector<std::string> scs1_name;
	std::transform(std::begin(scs1), std::end(scs1), std::back_inserter(scs1_name), [](const SubClass &sc){return sc.name;});
	
	std::unordered_map<term, frequency> terms {{1, 10}};
	Patent p1 = Patent::create(scs1_name, terms);
	
	std::vector<Class> ps1 = {"a01"_S, "a01"_S, "a02"_S};
	std::vector<Class> ps2 = {"b01"_S, "a01"_S, "a02"_S};
	std::vector<Class> ps3 = {"a01"_S, "b01"_S, "a02"_S};

	p1.predictions = std::move(ps1);
	assertTrue(e(p1));
	
	p1.predictions = std::move(ps2);
	assertFalse(e(p1));
	
	p1.predictions = std::move(ps3);
	assertTrue(e(p1));
}

void test_threeGuesses(const Evaluate &e){
	std::vector<SubClass> scs1 = {"a01a"_S, "a01b"_S};
	std::vector<std::string> scs1_name;
	std::transform(std::begin(scs1), std::end(scs1), std::back_inserter(scs1_name), [](const SubClass &sc){return sc.name;});
	
	std::unordered_map<term, frequency> terms {{1, 10}};
	Patent p1 = Patent::create(scs1_name, terms);
	
	std::vector<Class> ps1 = {"a01"_S, "a01"_S, "a02"_S};
	std::vector<Class> ps2 = {"b01"_S, "a01"_S, "a01"_S};
	std::vector<Class> ps3 = {"a02"_S, "b01"_S, "a02"_S};
	
	p1.predictions = std::move(ps1);
	assertTrue(e(p1));
	
	p1.predictions = std::move(ps2);
	assertTrue(e(p1));
	
	p1.predictions = std::move(ps3);
	assertFalse(e(p1));
}

void test_allCategories(const Evaluate &e){
	std::vector<SubClass> scs1 = {"a01a"_S, "a01b"_S};
	std::vector<std::string> scs1_name;
	std::transform(std::begin(scs1), std::end(scs1), std::back_inserter(scs1_name), [](const SubClass &sc){return sc.name;});
	
	std::unordered_map<term, frequency> terms {{1, 10}};
	Patent p1 = Patent::create(scs1_name, terms);
	
	std::vector<Class> ps1 = {"a01"_S, "a01"_S, "a02"_S};
	std::vector<Class> ps2 = {"b01"_S, "a01"_S, "a01"_S};
	std::vector<Class> ps3 = {"a01"_S, "b01"_S, "a02"_S};
	
	p1.predictions = std::move(ps1);
	assertTrue(e(p1));
	
	p1.predictions = std::move(ps2);
	assertFalse(e(p1));
	
	p1.predictions = std::move(ps3);
	assertTrue(e(p1));
}

void test_Evaluate_constructor(){
	Evaluate e1;
	test_topPrediction(e1);
	
	Evaluate e2{"topprediction"};
	test_topPrediction(e2);
	
	Evaluate e3{"top prediction"};
	test_topPrediction(e3);
	
	Evaluate e4{"threeguesses"};
	test_threeGuesses(e4);
	
	Evaluate e5{"three guesses"};
	test_threeGuesses(e5);
	
	Evaluate e6{"allcategories"};
	test_allCategories(e6);
	
	Evaluate e7{"all categories"};
	test_allCategories(e7);
	
	bool throwed = true;
	try{
		Evaluate e8{"wrong name"};
		throwed = false;
	} catch(const exception &e){}
	
	if(!throwed){
		throwException("expected an exceptrion for a wrong name");
	}
}

void test_Evaluate_single(){
	Evaluate e;
	test_topPrediction(e);
}

void test_Evaluate_multiple(){
	std::vector<SubClass> scs1 = {"a01a"_S, "a01b"_S};
	std::vector<std::string> scs1_name;
	std::transform(std::begin(scs1), std::end(scs1), std::back_inserter(scs1_name), [](const SubClass &sc){return sc.name;});
	std::unordered_map<term, frequency> terms {{1, 10}};
	Patent p1 = Patent::create(scs1_name, terms);
	Patent p2 = Patent::create(scs1_name, terms);
	Patent p3 = Patent::create(scs1_name, terms);
	Patent p4 = Patent::create(scs1_name, terms);
	
	std::vector<Class> ps1 = {"a01"_S, "a01"_S, "a02"_S};
	std::vector<Class> ps2 = {"b01"_S, "a01"_S, "a01"_S};
	std::vector<Class> ps3 = {"a01"_S, "b01"_S, "a02"_S};
	std::vector<Class> ps4 = {"a01"_S, "a01"_S, "d02"_S};
	
	p1.predictions = std::move(ps1);
	p2.predictions = std::move(ps2);
	p3.predictions = std::move(ps3);
	p4.predictions = std::move(ps4);
	
	PatentList patents = {p1, p2, p3, p4};
	
	Evaluate e;
	assertEquals(3, e(patents));
}

void runTest(const std::function<void()> &f){
	try{
		f();
	} catch(const exception &e){
		std::cout << "In " << e.origin << " -> " << e.msg << std::endl;
	} catch(...){
		std::cout << "Other exception happened" << std::endl;
	}
}

void runTests(){
	std::vector<std::function<void()>> tests;
	tests.push_back(test_SubClass_constructor);
	tests.push_back(test_SubClass_neq);
	tests.push_back(test_SubClass_eq);
	tests.push_back(test_SubClass_class);
	tests.push_back(test_SubClass_lt);
	
	tests.push_back(test_Class_constructor);
	tests.push_back(test_Class_neq);
	tests.push_back(test_Class_eq);
	tests.push_back(test_Class_section);
	
	tests.push_back(test_Section_constructor);
	tests.push_back(test_Section_neq);
	tests.push_back(test_Section_eq);
	
	tests.push_back(test_Patent_constructor);
	tests.push_back(test_Evaluate_constructor);
	tests.push_back(test_Evaluate_single);
	tests.push_back(test_Evaluate_multiple);

	for(const auto& test : tests){
		runTest(test);
	}
}

#endif
