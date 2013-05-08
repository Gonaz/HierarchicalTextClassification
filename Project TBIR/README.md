HierarchicalTextClassification
===============================================

Compiling
-----------------------------
This application requires a modern C++ compiler (C++11 should be supported).
Before you compile the program, it is best to set the values of "path", "trainFilename", "testFilename" and "outputFilename" to the correct values (these values are located in the main function in the main file). Also you should include the correct header in the main. There are 7 possibilities:

	1. tc1.h: Don't use the hierarchy at all, only use the classes
	2. tc2.h: Don't use the hierarchy at all, but uses the classes and the sections
	3. tc3.h: Don't use the hierarchy at all, but uses the classes and the sections. In the test phase, only terms with a frequency higher than a certain threshold are considered.
	4. tc4.h: Don't use the hierarchy at all, but uses the classes and the sections. In the test phase, only terms with a frequency higher than a certain threshold are considered, also terms with a higher frequency have more weight.
	5. tc5.h: Same thing as tc4.h, but uses the hierarchy to do a local search.
	6. tc6.h: Same thing as tc4.h, but in the training phase every subclass is considered (not only the first one).
	7. tc7.h: Same thing as tc5.h, but in the training phase every subclass is considered (not only the first one).

tc4.h is considered as the baseline and tc5.h is actually the improvement (it works faster).
To compile the program, execute the following command (if your C++ compiler isn't named g++-mp-4.8, you should change this file)
	
	bash compile.sh

Data
-------------------------------
The data used to train and the data used to test have the same structure. Each line of the file corresponds to a patent. First the subclasses of the patent are listed (separated by commas) after a space the frequency of each term is listed (separated by spaces). Here is an example of a patent with two subclasses (the first letter of a subclass is the section, the two digits are the class and the last letter indicates the subclass, this program only predicts the class).

	g01n,a61k 3:1 17:1 35:3 39:4 44:1 52:3 56:1 59:1 64:1 69:1 71:1 155:3 171:1 187:1 197:1 254:1 270:1 297:1 302:1 303:1 313:1 427:2 448:1 453:2 458:1 517:1 564:1 597:1 613:1 637:1 639:2 688:1 713:1 792:1 806:2 809:1 820:1 882:2 891:2 912:1 917:8 923:1 947:1 1012:1 1013:1 1021:1 1058:1 1154:2 1196:1 1218:1 1281:1 1284:1 1298:1 1359:2 1376:1 1392:1 1518:1 1567:1 1597:1 1688:1 1732:1 1733:1 1809:1 1956:1 1991:1 2032:1 2051:1 2071:1 2153:1 2156:3 2178:1 2202:1 2268:1 2269:1 2466:1 2519:1 2654:2 2691:2 2754:1 2771:1 3083:1 3258:4 3267:2 3467:1 3488:1 3492:1 3493:1 3540:3 3555:2 3562:1 3572:2 3622:1 3629:1 3657:1 3761:1 3783:5 3795:1 3816:1 3838:1 3953:1 4000:3 4078:3 4136:1 4310:1 4666:1 4865:1 4927:2 4963:1 5030:1 5317:1 5374:1 5432:1 5484:1 5587:1 5687:1 6005:2 6017:1 6057:1 6095:1 6246:1 6700:1 6736:1 6777:1 7046:1 7227:1 7383:1 7481:1 7887:1 7893:1 8217:2 8532:2 8555:1 8905:1 9487:1 10313:1 10371:1 10386:1 10397:1 10410:1 10414:4 10463:1 10468:1 10509:1 10597:1 10816:5 10822:7 10997:1 11650:1 11880:1 12690:1 12934:2 13377:1 13378:1 13381:1 13802:1 15014:1 15957:1 20112:5 21543:1 21544:1 23793:1 25241:2 25578:1 28226:1 28462:1 28774:1 29889:1 49645:1 49874:3 50331:1

Running
---------------------------
If the compilation was succesful, an executable names "tbir" should be created. Executing this program will train the model and test it. Just run the command

	./tbir
