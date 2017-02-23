#include"DataMan.h"


DataMan::DataMan(){
}

DataMan::~DataMan(){
  while(!cache_shape.empty()) {
      cache_shape.pop_back();
  }
}



