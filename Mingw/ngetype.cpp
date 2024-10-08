#include "ngetype.h"

/**
 * @brief Initialize the index buffer object
 *     Each 2D sprite can be rendered in a form of 2 Triangles (6 indicies).
 *     So this function prepares so that sprites can be loaded into memory
 * @param data pointer to the indicies 
 * @param _count amount of indicies (size of the indicies pointer divided by 6)
 */
ngetype::IBO::IBO(unsigned short int *data, int _count) : count(_count){
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned short int), data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
/**
 * @brief Initialize the buffer to load data
 * read more about glBindBuffer on https://docs.gl/gl3/glBindBuffer
 */
void ngetype::IBO::bind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);}
/**
 * @brief Unbind the buffer
 * read more about glBindBuffer on https://docs.gl/gl3/glBindBuffer
 */
void ngetype::IBO::unbind(){glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);}
/**
 * @brief Get amount of indicies buffered
 * 
 * @return unsigned int amount of indicies as integer
 */
unsigned int ngetype::IBO::getCount(){return count;}


