#include "ngetype.h"

///Constructors
ngetype::IBO::IBO(unsigned short int *data, int _count) : count(_count){
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned short int), data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void ngetype::IBO::bind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);}
void ngetype::IBO::unbind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);}
unsigned int ngetype::IBO::getCount(){return count;}


