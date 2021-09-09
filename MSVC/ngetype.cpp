#include "ngetype.h"

///Constructors
ngetype::VBO::VBO(float *data, int count, unsigned int _componentCount) : componentCount(_componentCount){
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(float), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ngetype::VBO::bind(){glBindBuffer(GL_ARRAY_BUFFER,id);}
void ngetype::VBO::unbind(){glBindBuffer(GL_ARRAY_BUFFER,0);}
unsigned int ngetype::VBO::getComponentCount(){return componentCount;}
///Constructors
ngetype::VAO::VAO(){
    glGenVertexArrays(1,&id);
}
void ngetype::VAO::addVBO(VBO* vbo, unsigned int index){
    bind();
    vbo->bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, vbo->getComponentCount(), GL_FLOAT, false, 0,0);
    vbo->unbind();
    unbind();
}
void ngetype::VAO::bind(){glBindVertexArray(id);}
void ngetype::VAO::unbind(){glBindVertexArray(0);}
///Constructors
ngetype::IBO::IBO(unsigned short int *data, int _count) : count(_count){
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned short int), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void ngetype::IBO::bind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);}
void ngetype::IBO::unbind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);}
unsigned int ngetype::IBO::getCount(){return count;}


