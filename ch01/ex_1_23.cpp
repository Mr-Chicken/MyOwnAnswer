#include <iostream>
#include "Sales_item.h"
// compile and  use "./a.out <book.txt>log"
using namespace std;
int main() {
    Sales_item cur,val;
    if (cin >> cur) {
	while (cin >> val) {
	    if (cur.isbn() == val.isbn()) {
		cur += val;
		cout << "Cur is now: " << cur << endl;
	    } else {
		cout <<"Cur was: "<< cur << endl;
		cur = val;  
	    }
	}
    } else {
	cout << "No Data? Ok i am leaving!";
    }
    
    return 0;
}
