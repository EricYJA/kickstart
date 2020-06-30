#include <iostream>

#include "seqList.h"


int main(){
  std::cout << "Hello C++ data stuctures" << std::endl;

  // Test for seqList
  SeqList<int> test(15);
	int array[15]={2,5,8,1,9,9,7,6,4,3,2,9,7,7,9};
	for(int i=0;i<15;i++){
		test.Insert(array[i],0);
	}
	test.Insert(1,0);
	std::cout << (test.Find(0) ? "can't be found " : "Be found ") << 0 << std::endl << std::endl;
	test.Remove(7);
	test.Print();
	test.Remove(9);
	test.Print();
	test.Remove(0);
	test.Print();


  return 0;
}
