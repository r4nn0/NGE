#include "Renderer3D.h"
#include <glm/gtx/string_cast.hpp>
#define MEGABYTE 1048576
#define VERTEX_SIZE3D sizeof(vboData)
#define BUFFER_SIZE3D 30*MEGABYTE
#define INDICES_SIZE3D 3*BUFFER_SIZE3D
#define MODEL_MATRICES_SSBO_SIZE3D 2*MEGABYTE
#define MORPH_POSITIONS_SSBO_SIZE3D 2*MEGABYTE
#define JOINT_MATRICES_SSBO_SIZE3D 2*MEGABYTE
#define MORPH_WEIGHTS_SSBO_SIZE3D MEGABYTE
#define NODE_MATRICES_SSBO_SIZE3D 2*MEGABYTE

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer3D::Renderer3D() {
    glCreateVertexArrays(1, &m_appSurface);


    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, BUFFER_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, INDICES_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT );

    glCreateBuffers(1, &m_ModelMatricesSSBO);
    glNamedBufferStorage(m_ModelMatricesSSBO, MODEL_MATRICES_SSBO_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &m_MorphPositionsSSBO);
    glNamedBufferStorage(m_MorphPositionsSSBO, MORPH_POSITIONS_SSBO_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &m_JointMatricesSSBO);
    glNamedBufferStorage(m_JointMatricesSSBO, JOINT_MATRICES_SSBO_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &m_MorphWeightsSSBO);
    glNamedBufferStorage(m_MorphWeightsSSBO, MORPH_WEIGHTS_SSBO_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &m_NodeMatricesSSBO);
    glNamedBufferStorage(m_NodeMatricesSSBO, NODE_MATRICES_SSBO_SIZE3D, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glVertexArrayVertexBuffer(m_appSurface,0,m_VBO,0, VERTEX_SIZE3D);
    glVertexArrayElementBuffer(m_appSurface, m_IBO);


    for(int i=0;i<=14;i++){
        glEnableVertexArrayAttrib(m_appSurface,i);
        glVertexArrayAttribBinding(m_appSurface,i,0);
    }
    /*
    std::cout << sizeof(vboData) << std::endl;
    std::cout << offsetof(vboData,pos) << std::endl;
    std::cout << offsetof(vboData,color) << std::endl;
    std::cout << offsetof(vboData,texCoords) << std::endl;
    std::cout << offsetof(vboData,vNormals) << std::endl;
    std::cout << offsetof(vboData,textureSlot) << std::endl;
    std::cout << offsetof(vboData,modelID) << std::endl;
    std::cout << offsetof(vboData,aJoints) << std::endl;
    std::cout << offsetof(vboData,aWeights) << std::endl;
    std::cout << offsetof(vboData,mtc) << std::endl;
    std::cout << offsetof(vboData,vertexIndex) << std::endl;*/
    /*
    glm::vec3 pos;
        glm::vec4 color;
        glm::vec2 texCoords;
        glm::vec3 vNormals;
        unsigned textureSlot;
        unsigned modelID;
        glm::uvec4 aJoints;
        glm::vec4 aWeights;
        unsigned mtc;
        unsigned mwo;
        unsigned mpo;
        unsigned jo; */
    glVertexArrayAttribFormat(m_appSurface, 0, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, pos));
glVertexArrayAttribFormat(m_appSurface, 1, 4, GL_FLOAT, GL_FALSE, offsetof(vboData, color));
glVertexArrayAttribFormat(m_appSurface, 2, 2, GL_FLOAT, GL_FALSE, offsetof(vboData, texCoords));
glVertexArrayAttribFormat(m_appSurface, 3, 3, GL_FLOAT, GL_FALSE, offsetof(vboData, vNormals));

glVertexArrayAttribIFormat(m_appSurface, 4, 1, GL_INT, offsetof(vboData, textureSlot));
glVertexArrayAttribIFormat(m_appSurface, 5, 1, GL_UNSIGNED_INT, offsetof(vboData, modelID));
glVertexArrayAttribIFormat(m_appSurface, 6, 4, GL_UNSIGNED_INT, offsetof(vboData, aJoints));

glVertexArrayAttribFormat(m_appSurface, 7, 4, GL_FLOAT, GL_FALSE, offsetof(vboData, aWeights));

glVertexArrayAttribIFormat(m_appSurface, 8, 1, GL_UNSIGNED_INT, offsetof(vboData, mtc));
glVertexArrayAttribIFormat(m_appSurface, 9, 1, GL_UNSIGNED_INT, offsetof(vboData, mwo));
glVertexArrayAttribIFormat(m_appSurface, 10, 1, GL_UNSIGNED_INT, offsetof(vboData, mpo));
glVertexArrayAttribIFormat(m_appSurface, 11, 1, GL_UNSIGNED_INT, offsetof(vboData, jo));
glVertexArrayAttribIFormat(m_appSurface, 12, 1, GL_UNSIGNED_INT, offsetof(vboData, vertexIndex));
glVertexArrayAttribIFormat(m_appSurface, 13, 1, GL_UNSIGNED_INT, offsetof(vboData, vertexCount));
glVertexArrayAttribIFormat(m_appSurface, 14, 1, GL_UNSIGNED_INT, offsetof(vboData, nodeMatrixIndex));
//std::cout << "sizeof(vboData): " << sizeof(vboData) << std::endl;
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
    
    //const unsigned char limit = (unsigned char)(rand() % 231 + 25);
    // Randomly generate an unsigned char per RGB channel
    /*
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
    //glMakeTextureHandleResidentARB(texHandle);
    if(glGetError()!=GL_NO_ERROR){
        std::cout << "Error making texture handle resident" << std::endl;
    }
    */
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
    glDeleteBuffers(1,&m_ModelMatricesSSBO);
    glDeleteBuffers(1,&m_MorphPositionsSSBO);
    glDeleteBuffers(1,&m_JointMatricesSSBO);
    glDeleteBuffers(1,&m_MorphWeightsSSBO);
    glDeleteBuffers(1,&m_NodeMatricesSSBO);
    glDeleteProgram(m_Shader);
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer3D::Render(){
    GLsizei m_indexCount = 0, instanceCount=0, jointOffset=0, morphWeightsOffset=0, morphPositionsOffset=0;

    vboData* m_Buff = (vboData*) glMapNamedBuffer(m_VBO, GL_WRITE_ONLY);
    unsigned int* indexPtr = (unsigned int*)glMapNamedBuffer(m_IBO, GL_WRITE_ONLY);
    glm::mat4* mmSSBOptr = (glm::mat4*) glMapNamedBuffer(m_ModelMatricesSSBO, GL_WRITE_ONLY);
    glm::vec3* mpSSBOptr = (glm::vec3*) glMapNamedBuffer(m_MorphPositionsSSBO, GL_WRITE_ONLY);
    glm::mat4* jmSSBOptr = (glm::mat4*) glMapNamedBuffer(m_JointMatricesSSBO, GL_WRITE_ONLY);
    float* mwSSBOptr = (float*) glMapNamedBuffer(m_MorphWeightsSSBO, GL_WRITE_ONLY);
    glm::mat4* nmSSBOptr = (glm::mat4*) glMapNamedBuffer(m_NodeMatricesSSBO, GL_WRITE_ONLY);

    if (!m_Buff || !indexPtr || !mmSSBOptr || !mpSSBOptr || !jmSSBOptr || !mwSSBOptr) {
        std::cerr << "Failed to map buffer!" << std::endl;
        return;
    }
    unsigned int vboOffset = 0;
    unsigned lastObjectSkinOffset = 0;
    unsigned lastMorphWeightOffset = 0;
    unsigned nodeMatrixOffset = 0;
    for(Object3D& obj:ObjectsToRender){
        std::map<int, int> skinOffsets;
        std::vector<int> morphWeightsOffsets;
        GLsizei localJointOffset=0;
        mmSSBOptr[instanceCount] = obj.modelMatrix;
        unsigned nodeMatrixBaseOffset = nodeMatrixOffset;
        memcpy(nmSSBOptr + nodeMatrixOffset, obj.nodeGlobalMatrix.data(), obj.nodeGlobalMatrix.size() * sizeof(glm::mat4));
        nodeMatrixOffset += obj.nodeGlobalMatrix.size();


        for(Object3D::Skin& skin: obj.skins){
            skinOffsets[skin.skinIndex] =  localJointOffset + lastObjectSkinOffset;
            memcpy(jmSSBOptr + jointOffset, skin.finalJointMatrices.data(), skin.finalJointMatrices.size()* sizeof(glm::mat4));
            localJointOffset += skin.finalJointMatrices.size();
            jointOffset+=skin.finalJointMatrices.size();
        }
        lastObjectSkinOffset += localJointOffset;
        //morphWeightsOffsets.push_back(morphWeightsOffset + lastMorphWeightOffset);
        for(std::vector<float>& morphWeights : obj.morphWeights){
            morphWeightsOffsets.push_back(morphWeightsOffset);
            memcpy(mwSSBOptr + morphWeightsOffset, morphWeights.data(), morphWeights.size() * sizeof(float));
            morphWeightsOffset+=morphWeights.size();
            
        }
        
        //lastMorphWeightOffset = morphWeightsOffset;
        //int primID = 0;
        unsigned lastMorphPositionsOffset = 0;
        for(Object3D::Primitive& prim : obj.primitives){
            for(std::vector<glm::vec3>& morphPositions : prim.morphPositions){
                
                memcpy(mpSSBOptr + morphPositionsOffset, morphPositions.data(), morphPositions.size()* sizeof(glm::vec3));
                morphPositionsOffset+=morphPositions.size();
            }
            std::vector<unsigned int> ind = prim.indices;
            for(unsigned i = 0; i<ind.size();i++){
                indexPtr[m_indexCount++] = ind[i] + vboOffset;
                
                //std::cout << ind[i] << std::endl;
            }
            unsigned localVertexIndex = 0;
            for(const Object3D::Vertex3D& vertex: prim.vertices){
                
                vboData currentVertex;
                //std::cout << vertex.pos.x << ",\t" << vertex.pos.y << ",\t" << vertex.pos.z << std::endl;
                currentVertex = vertex;
                currentVertex.modelID = instanceCount;
                currentVertex.mtc = prim.morphPositions.size();
                if(prim.nodeIndex<morphWeightsOffsets.size())
                    currentVertex.mwo = morphWeightsOffsets[prim.nodeIndex];
                else currentVertex.mwo = 0;
                currentVertex.mpo = lastMorphPositionsOffset;

                if(prim.skinIndex<skinOffsets.size()) currentVertex.jo = skinOffsets[prim.skinIndex];
                else currentVertex.jo=0;

                currentVertex.vertexIndex = localVertexIndex++;
                currentVertex.vertexCount = prim.vertices.size();
                currentVertex.nodeMatrixIndex = nodeMatrixBaseOffset+prim.nodeIndex;
                m_Buff[vboOffset++] = currentVertex;
            }
            //primID++;
            lastMorphPositionsOffset = morphPositionsOffset;
        }
        
        instanceCount++;
    }
    //std::cout << "Vertex Count: " << vboOffset <<std::endl;
    
    glUnmapNamedBuffer(m_VBO);
    glUnmapNamedBuffer(m_IBO);
    glUnmapNamedBuffer(m_ModelMatricesSSBO);
    glUnmapNamedBuffer(m_MorphPositionsSSBO);
    glUnmapNamedBuffer(m_JointMatricesSSBO);
    glUnmapNamedBuffer(m_MorphWeightsSSBO);
    glUnmapNamedBuffer(m_NodeMatricesSSBO);
    ObjectsToRender.clear();
    
    //glMakeTextureHandleResidentARB(texHandle);
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ModelMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_MorphPositionsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_JointMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_MorphWeightsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_NodeMatricesSSBO);
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, texBuffer);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "proj_matrix"),1,GL_FALSE,Engine::getProjMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "vw_matrix"),1,GL_FALSE,Engine::getViewMatrix());
    
    glUniform1i(glGetUniformLocation(m_Shader, "texture[0]"), MainTextureAtlas.GetTextureSlot());
    glUniform3f(glGetUniformLocation(m_Shader, "lightPos"), Engine::camera3d.getPosition().x, Engine::camera3d.getPosition().y, Engine::camera3d.getPosition().z);
    glUniform3f(glGetUniformLocation(m_Shader, "viewPos"), Engine::camera3d.getPosition().x, Engine::camera3d.getPosition().y, Engine::camera3d.getPosition().z);
    glUniform3f(glGetUniformLocation(m_Shader, "lightColor"), 1.0f, 1.0f, 1.0f);
    
    
    MainTextureAtlas.Bind();
    glBindVertexArray(m_appSurface);
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);
    
    //glMakeTextureHandleNonResidentARB(texHandle);
    
    glBindVertexArray(0);
    MainTextureAtlas.Unbind();
    
    glUseProgram(0);
}
