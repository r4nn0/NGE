#include "Renderer3D.h"

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer3D::Renderer3D() : dcpf(0) {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextures);
    glGenVertexArrays(1, &m_appSurface);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_appSurface);
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 1000000*sizeof(ngetype::vbo3DData), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    //glEnableVertexAttribArray(5);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)0); // vec3 pos
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*3));  // vec4 col
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*7));  // vec2 texcoord
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*9));  // vec3 vNormals
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*12)); // float textureSlot
    /* MATRIX 4x4 Model */
    for(int i=0;i<4;i++){
        glEnableVertexAttribArray(5 + i);
        glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i + (sizeof(float)*13)));
        glVertexAttribDivisor(5 + i, 1);
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    m_indexBuffer = new ngetype::IBO(NULL, 1000000*4);
    glBindVertexArray(0);
    m_Shader=Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/shader3D.vs").c_str(),
                                  Engine::LoadShaderFromFile("./Graphics/Shaders/shader3D.fs").c_str());
}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer3D::~Renderer3D() {
    delete m_indexBuffer;
    glDeleteBuffers(1,&m_VBO);
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer3D::Render(){
    m_indexCount = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_indexBuffer->bind();

    m_Buff = (ngetype::vbo3DData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    unsigned int* indexPtr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!m_Buff || !indexPtr) {
        std::cerr << "Failed to map buffer!" << std::endl;
        return;
    }
    unsigned int vboOffset = 0;
    for(Object3D& obj:ObjectsToRender){
        
        for(const Object3D::Vertex3D& vertex:obj.getVertices()){
            ngetype::vbo3DData vboData;
            // Multiply the matrix by the position before passing to the shader
            //glm::vec4 pos = obj.getModelMatrix()*glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0);
            //vboData.vertex= glm::vec3(pos.x, pos.y, pos.z);
            vboData.vertex= vertex.pos;
            vboData.color=vertex.color;
            vboData.texCoords=vertex.texCoords;
            vboData.vNormals= vertex.normal;
            vboData.model = obj.getModelMatrix();
            vboData.textureSlot=vertex.textureSlot;
            m_Buff[vboOffset++] = vboData;
            //std::cout << vboData.vertex.x << std::endl;
        }
        for(unsigned int index:obj.getIndices()){
            indexPtr[m_indexCount++] = index + vboOffset - obj.getVertices().size();
        }
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ObjectsToRender.clear();
    glUseProgram(m_Shader);
    //glm::mat4 modelMatrix = glm::mat4(1.0f);
    /*
    float angle = glfwGetTime();
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));*/
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getProjMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix());
    //glUniformMatrix4fv(glGetUniformLocation(m_Shader, "ml_matrix"),1,GL_FALSE,glm::value_ptr(modelMatrix));
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    //glUniform3f(glGetUniformLocation(m_Shader, "lightPos"), 1.2f, 10.0f, 2.0f);
    //glUniform3f(glGetUniformLocation(m_Shader, "viewPos"), Engine::camera3d.getPosition().x, Engine::camera3d.getPosition().y, Engine::camera3d.getPosition().z);
    //glUniform3f(glGetUniformLocation(m_Shader, "lightColor"), 1.0f, 1.0f, 1.0f);
    //glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    //MainTextureAtlas.Bind();
    MainTextureAtlas.Bind();
    glBindVertexArray(m_appSurface);
    m_indexBuffer->bind();
    //std::cout << "Index count " << m_indexCount << std::endl;
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);
    
    m_indexBuffer->unbind();
    glBindVertexArray(0);
    MainTextureAtlas.Unbind();
    dcpf++;
    //MainTextureAtlas.Unbind();
    glUseProgram(0);
}
