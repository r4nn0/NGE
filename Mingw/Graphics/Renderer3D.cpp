#include "Renderer3D.h"
#include <glm/gtx/string_cast.hpp>
/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer3D::Renderer3D() : dcpf(0) {
    glCreateVertexArrays(1, &m_appSurface);


    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, 450000*sizeof(ngetype::vbo3DData), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, 1000000*8, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT );

    glCreateBuffers(1, &m_SSBO);
    glNamedBufferStorage(m_SSBO, sizeof(glm::mat4) * 150000, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glVertexArrayVertexBuffer(m_appSurface,0,m_VBO,0,sizeof(ngetype::vbo3DData));
    glVertexArrayElementBuffer(m_appSurface, m_IBO);


    for(int i=0;i<6;i++){
        glEnableVertexArrayAttrib(m_appSurface,i);
        glVertexArrayAttribBinding(m_appSurface,i,0);
    }
    glVertexArrayAttribFormat(m_appSurface, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_appSurface, 1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3);
    glVertexArrayAttribFormat(m_appSurface, 2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7);
    glVertexArrayAttribFormat(m_appSurface, 3, 3, GL_FLOAT, GL_FALSE, sizeof(float)*9);
    glVertexArrayAttribFormat(m_appSurface, 4, 3, GL_FLOAT, GL_FALSE, sizeof(float)*12);
    glVertexArrayAttribFormat(m_appSurface, 5, 3, GL_FLOAT, GL_FALSE, sizeof(float)*13);

    
    /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)0); // vec3 pos
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*3));  // vec4 col
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*7));  // vec2 texcoord
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*9));  // vec3 vNormals
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*12)); // float textureSlot
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(float)*13)); // unsigned modelID
    */
    
    //glGenBuffers(1, &m_SSBO);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBO);
    //glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 150000, nullptr, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    /*
    // MATRIX 4x4 Model 
    for(int i=0;i<4;i++){
        glVertexAttribPointer(5 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i + (sizeof(float)*13)));
        glVertexAttribDivisor(5 + i, 1);
    }
    
    
    // MATRIX 4x4 Model 
    for(int i=0;i<4;i++){
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glVertexAttribDivisor(i, 1);
    }
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(glm::vec4)*3)); // vec3 pos
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(glm::vec4)*4+sizeof(float)*3));  // vec4 col
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(glm::vec4)*4+sizeof(float)*7));  // vec2 texcoord
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(glm::vec4)*4+sizeof(float)*9));  // vec3 vNormals
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(ngetype::vbo3DData), (const void*)(sizeof(glm::vec4)*4+sizeof(float)*12)); // float textureSlot
    */
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //m_indexBuffer = new ngetype::IBO(NULL, 1000000*4);
    
    //glGenTextures(1, &testTex);
    
    const unsigned char limit = (unsigned char)(rand() % 231 + 25);
    // Randomly generate an unsigned char per RGB channel
    unsigned char textureData[32*32*3];
    for (int j = 0; j < 32*32*3; ++j) {
        textureData[j] = 150;
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &testTex);
    glTextureStorage2D(testTex, 1, GL_RGB8, 32,32);
    glTextureSubImage2D(
        testTex, 
        // level, xoffset, yoffset, width, height
        0, 0, 0, 32, 32, 
        GL_RGB8, GL_UNSIGNED_BYTE, 
        (const void *)textureData);


    texHandle=glGetTextureHandleARB(testTex);
    //glMakeTextureHandleResidentARB(texHandle);
    //glMakeTextureHandleNonResidentARB(texHandle); //make texture removable from residency list
    
    glCreateBuffers(1, &texBuffer);
    glNamedBufferStorage(texBuffer, sizeof(GLuint64), (const void*)(&texHandle), GL_DYNAMIC_STORAGE_BIT);
    glMakeTextureHandleResidentARB(texHandle);
    if(glGetError()!=GL_NO_ERROR){
        std::cout << "Error making texture handle resident" << std::endl;
    }
    
    m_Shader=Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/shader3D.vs").c_str(),
                                  Engine::LoadShaderFromFile("./Graphics/Shaders/shader3D.fs").c_str());
}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer3D::~Renderer3D() {
    glDeleteBuffers(1,&m_VBO);
    glDeleteBuffers(1,&m_IBO);
    glDeleteBuffers(1,&m_SSBO);
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer3D::Render(){
    GLsizei m_indexCount = 0, instanceCount=0;

    ngetype::vbo3DData* m_Buff = (ngetype::vbo3DData*) glMapNamedBuffer(m_VBO, GL_WRITE_ONLY);
    unsigned int* indexPtr = (unsigned int*)glMapNamedBuffer(m_IBO, GL_WRITE_ONLY);
    glm::mat4* ssboBuffer = (glm::mat4*) glMapNamedBuffer(m_SSBO, GL_WRITE_ONLY);
    

    if (!m_Buff || !indexPtr || !ssboBuffer) {
        std::cerr << "Failed to map buffer!" << std::endl;
        return;
    }
    unsigned int vboOffset = 0;
    for(Object3D& obj:ObjectsToRender){
        //glm::mat4 modelMatrix = obj.getModelMatrix();
        ssboBuffer[instanceCount] = obj.getModelMatrix();
        //glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4)*instanceCount, sizeof(glm::mat4), &modelMatrix);
        //instanceCount++;
        std::vector<unsigned int> ind = obj.getIndices();
        for(unsigned i = 0; i<ind.size();i++){
            indexPtr[m_indexCount++] = ind[i] + vboOffset;
        }
        for(const Object3D::Vertex3D& vertex:obj.getVertices()){
            ngetype::vbo3DData vboData;
            // Multiply the matrix by the position before passing to the shader
            //glm::vec4 pos = obj.getModelMatrix()*glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0);
            //vboData.vertex= glm::vec3(pos.x, pos.y, pos.z);
            vboData.vertex= vertex.pos;
            vboData.color= vertex.color;
            vboData.texCoords=vertex.texCoords;
            vboData.vNormals= vertex.normal;
            vboData.textureSlot=vertex.textureSlot;
            vboData.modelID = instanceCount;
            m_Buff[vboOffset++] = vboData;
        }
        instanceCount++;
    }
    
    glUnmapNamedBuffer(m_VBO);
    glUnmapNamedBuffer(m_IBO);
    glUnmapNamedBuffer(m_SSBO);
    
    ObjectsToRender.clear();
    
    //glMakeTextureHandleResidentARB(texHandle);
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_SSBO);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, texBuffer);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getProjMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix());
    
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    glUniform3f(glGetUniformLocation(m_Shader, "lightPos"), Engine::camera3d.getPosition().x, Engine::camera3d.getPosition().y, Engine::camera3d.getPosition().z);
    glUniform3f(glGetUniformLocation(m_Shader, "viewPos"), Engine::camera3d.getPosition().x, Engine::camera3d.getPosition().y, Engine::camera3d.getPosition().z);
    glUniform3f(glGetUniformLocation(m_Shader, "lightColor"), 1.0f, 1.0f, 1.0f);
    
    
    //MainTextureAtlas.Bind();
    glBindVertexArray(m_appSurface);
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);
    
    //glMakeTextureHandleNonResidentARB(texHandle);
    
    glBindVertexArray(0);
    
    //MainTextureAtlas.Unbind();
    
    dcpf++;
    glUseProgram(0);
}
