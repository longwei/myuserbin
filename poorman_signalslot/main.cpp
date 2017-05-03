#include "object.h"  
int main()  
{  
    Object obj1, obj2;  
    Object::connect(&obj1, "sig1", &obj2, "slot1");  
    obj1.testSignal();  
    return 0;;  
}