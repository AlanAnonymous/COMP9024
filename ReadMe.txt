说明：
  代码大概215行
  备注大概380行
  测试大概1000行


此assignment常见错误：
  1. Segmentation fault: 当指针是NULL的情况下，对其进行不恰当的操作，将会出现这种错误，所以检查指针
  2. 程序没输出： 一般都是当指针为NULL的情况下，用了->prev，->next或->data，则将会出现这种错误

一般解决方法： 
  加上判断指针是否为NULL的条件，例：
  if(pointer != NULL){
    XXXXXXX;
  }
  或 
  if(pointer){
    XXXXXXX;
  }

