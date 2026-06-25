#include "Renderer3D.h"
#include <glm/gtx/string_cast.hpp>
#define MEGABYTE 1048576
#define VERTEX_SIZE3D sizeof(Vertex3D)
#define BUFFER_SIZE3D 1024*MEGABYTE
#define INDICES_SIZE3D BUFFER_SIZE3D/4
#define MODEL_MATRICES_SSBO_SIZE3D 128*MEGABYTE
#define MORPH_POSITIONS_SSBO_SIZE3D 128*MEGABYTE
#define JOINT_MATRICES_SSBO_SIZE3D 64*MEGABYTE
#define MORPH_WEIGHTS_SSBO_SIZE3D 32*MEGABYTE
#define NODE_MATRICES_SSBO_SIZE3D 128*MEGABYTE
#define MATERIAL_SSBO_SIZE_3D 64*MEGABYTE

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer3D::Renderer3D() : vboOffset(0), m_indexCount(0), m_instanceCount(0),m_morphPositionsOffset(0), m_jointOffset(0), m_morphWeightsOffset(0), m_nodeMatrixOffset(0), m_materialOffset(1), m_drawDataOffset(0) , m_fence(nullptr), window_size(0){
    glGenQueries(2, m_timerQuery);
    glCreateVertexArrays(1, &m_VAO);

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, BUFFER_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    
    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, INDICES_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT );

    glCreateBuffers(1, &m_ModelMatricesSSBO);
    glNamedBufferStorage(m_ModelMatricesSSBO, MODEL_MATRICES_SSBO_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1, &m_MorphPositionsSSBO);
    glNamedBufferStorage(m_MorphPositionsSSBO, MORPH_POSITIONS_SSBO_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1, &m_JointMatricesSSBO);
    glNamedBufferStorage(m_JointMatricesSSBO, JOINT_MATRICES_SSBO_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1, &m_MorphWeightsSSBO);
    glNamedBufferStorage(m_MorphWeightsSSBO, MORPH_WEIGHTS_SSBO_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1, &m_NodeMatricesSSBO);
    glNamedBufferStorage(m_NodeMatricesSSBO, NODE_MATRICES_SSBO_SIZE3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1, &m_MaterialsSSBO);
    glNamedBufferStorage(m_MaterialsSSBO, MATERIAL_SSBO_SIZE_3D, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glCreateBuffers(1, &m_DrawDataSSBO);
    glNamedBufferStorage(m_DrawDataSSBO, 5*MEGABYTE, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glVertexArrayVertexBuffer(m_VAO,0,m_VBO,0, VERTEX_SIZE3D);
    glVertexArrayElementBuffer(m_VAO, m_IBO);



    m_vboBase = glMapNamedBufferRange(m_VBO, 0, BUFFER_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_indexBase = glMapNamedBufferRange(m_IBO, 0, INDICES_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_mmSSBOBase = glMapNamedBufferRange(m_ModelMatricesSSBO, 0, MODEL_MATRICES_SSBO_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_mpSSBOBase = glMapNamedBufferRange(m_MorphPositionsSSBO, 0, MORPH_POSITIONS_SSBO_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_jmSSBOBase = glMapNamedBufferRange(m_JointMatricesSSBO, 0, JOINT_MATRICES_SSBO_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_mwSSBOBase =  glMapNamedBufferRange(m_MorphWeightsSSBO, 0, MORPH_WEIGHTS_SSBO_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_nmSSBOBase = glMapNamedBufferRange(m_NodeMatricesSSBO, 0, NODE_MATRICES_SSBO_SIZE3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_materialSSBOBase = glMapNamedBufferRange(m_MaterialsSSBO, 0, MATERIAL_SSBO_SIZE_3D, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    m_ddSSBOBase = glMapNamedBufferRange(m_DrawDataSSBO, 0, 2*MEGABYTE, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    
    for(int i=0;i<=7;i++){
        glEnableVertexArrayAttrib(m_VAO,i);
        glVertexArrayAttribBinding(m_VAO,i,0);
    }
    
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, pos));
    glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, texCoords));
    glVertexArrayAttribFormat(m_VAO, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex3D, color));
    glVertexArrayAttribFormat(m_VAO, 3, 2, GL_UNSIGNED_SHORT, GL_TRUE, offsetof(Vertex3D, normal));

    glVertexArrayAttribIFormat(m_VAO, 4, 4, GL_UNSIGNED_BYTE, offsetof(Vertex3D, joints));
    glVertexArrayAttribFormat(m_VAO,  5, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex3D, weights));

    glVertexArrayAttribIFormat(m_VAO, 6, 1, GL_UNSIGNED_INT, offsetof(Vertex3D, vertexIndex));
    glVertexArrayAttribIFormat(m_VAO, 7, 1, GL_UNSIGNED_INT, offsetof(Vertex3D, drawID));

    m_Shader=CreateShader(LoadShaderFromFile("./Graphics/Shaders/shader3D.vs").c_str(),
                          LoadShaderFromFile("./Graphics/Shaders/shader3D.fs").c_str());
    
    m_uProjMatrix      = glGetUniformLocation(m_Shader, "proj_matrix");
    m_uVwMatrix        = glGetUniformLocation(m_Shader, "vw_matrix");
    
    m_uViewPos         = glGetUniformLocation(m_Shader, "uViewPos");
    m_uLightColor      = glGetUniformLocation(m_Shader, "uLightColor");
    m_uLightPos        = glGetUniformLocation(m_Shader, "uLightPos");
    m_uLightDir        = glGetUniformLocation(m_Shader, "uLightDir");
    m_uLightIntensity = glGetUniformLocation(m_Shader, "uLightIntensity");
    m_uAmbientStrength = glGetUniformLocation(m_Shader, "uAmbientStrength");
    m_uLightMode       = glGetUniformLocation(m_Shader, "uLightMode");

}
/**
 * @brief Frees the buffers in memory
 * 
 */
Renderer3D::~Renderer3D() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_ModelMatricesSSBO);
    glDeleteBuffers(1, &m_MorphPositionsSSBO);
    glDeleteBuffers(1, &m_JointMatricesSSBO);
    glDeleteBuffers(1, &m_MorphWeightsSSBO);
    glDeleteBuffers(1, &m_NodeMatricesSSBO);
    glDeleteBuffers(1, &m_MaterialsSSBO);
    glDeleteBuffers(1, &m_DrawDataSSBO);
    glDeleteProgram(m_Shader);
    
    // Shutdown TextureManager
    TextureManager::getInstance().shutdown();
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer3D::Render(){
    
    window_size = Engine::getInstance().getWindowSize();
    //static bool firstFlush = true;
    std::vector<unsigned> OpaqueMask, blend;
    glm::vec3 minPos(FLT_MAX), maxPos(-FLT_MAX);
    //GLsizei instanceCount=0, jointOffset=0, morphWeightsOffset=0, morphPositionsOffset=0, drawDataOffset = 0;
    
    Vertex3D* m_Buff = (Vertex3D*)m_vboBase;
    unsigned * indexPtr = (unsigned*)m_indexBase;
    glm::mat4* mmSSBOptr = (glm::mat4*)m_mmSSBOBase;
    glm::vec3* mpSSBOptr = (glm::vec3*)m_mpSSBOBase;
    glm::mat4* jmSSBOptr = (glm::mat4*)m_jmSSBOBase;
    float* mwSSBOptr = (float*)m_mwSSBOBase;
    glm::mat4* nmSSBOptr = (glm::mat4*)m_nmSSBOBase;
    Material* materialSSBOptr = (Material*) m_materialSSBOBase;
    DrawData* ddSSBOptr = (DrawData*)m_ddSSBOBase;
    
    if (!m_Buff || !indexPtr || !mmSSBOptr || !mpSSBOptr || !jmSSBOptr || !mwSSBOptr || !nmSSBOptr || !materialSSBOptr || !ddSSBOptr) {
        //printf("%p\n", indexPtr );
        std::cerr << "Failed to map one of the buffer!" << std::endl;
        printf("VBO: %p\nIBO: %p\nModelMatrixSSBO: %p\nMorphPositionsSSBO: %p\nJointMatrixSSBO: %p\nMorphWeightsSSBO: %p\nNodeMatrixSSBO: %p\nMaterialSSBO: %p\nDrawDataSSBO: %p\n"
               ,m_Buff, indexPtr, mmSSBOptr, mpSSBOptr, jmSSBOptr, mwSSBOptr, nmSSBOptr, nmSSBOptr, materialSSBOptr, ddSSBOptr);
        return;
    }
    
    if (m_fence) {
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence = nullptr;
    }
    
    Material defaultMat{};
    materialSSBOptr[0] = defaultMat;
    vboOffset = m_indexCount = m_instanceCount = m_morphPositionsOffset = m_jointOffset = m_morphWeightsOffset =  m_nodeMatrixOffset = m_materialOffset = m_drawDataOffset = 0;
    for(Object3D& obj : Objects3DToRender){
        if(obj.instanceID==-1) obj.instanceID=m_instanceCount;
        mmSSBOptr[obj.instanceID] = obj.modelMatrix; 
        
        std::map<int, int> skinOffsets;
        std::vector<int> morphWeightsOffsets;
        //GLsizei localJointOffset=0;
        
        unsigned nodeMatrixBaseOffset = m_nodeMatrixOffset;
        memcpy(nmSSBOptr + m_nodeMatrixOffset, obj.nodeGlobalMatrix.data(), obj.nodeGlobalMatrix.size() * sizeof(glm::mat4));
        m_nodeMatrixOffset += obj.nodeGlobalMatrix.size();


        for(Object3D::Skin& skin: obj.skins){
            skinOffsets[skin.skinIndex] =  m_jointOffset;
            memcpy(jmSSBOptr + m_jointOffset, skin.finalJointMatrices.data(), skin.finalJointMatrices.size()* sizeof(glm::mat4));
            //localJointOffset += skin.finalJointMatrices.size();
            m_jointOffset+=skin.finalJointMatrices.size();
        }
        //lastObjectSkinOffset += localJointOffset;
        //morphWeightsOffsets.push_back(morphWeightsOffset + lastMorphWeightOffset);
        for(std::vector<float>& morphWeights : obj.morphWeights){
            morphWeightsOffsets.push_back(m_morphWeightsOffset);
            memcpy(mwSSBOptr + m_morphWeightsOffset, morphWeights.data(), morphWeights.size() * sizeof(float));
            m_morphWeightsOffset+=morphWeights.size();
            
        }
        
        //lastMorphWeightOffset = morphWeightsOffset;
        //int primID = 0;
        
        
        for(Object3D::Primitive& prim : obj.primitives){
            int materialDefaultSlot = -1;
            if(prim.materialIndex>=0 && prim.materialIndex<(int)obj.materials.size()){
                if(obj.materials[prim.materialIndex].alphaMode != 2) {
                    for(auto idx : prim.indices) OpaqueMask.push_back(idx + vboOffset);
                }
                else{
                    for(auto idx : prim.indices) blend.push_back(idx + vboOffset);
                }
                materialDefaultSlot = m_materialOffset;
                materialSSBOptr[m_materialOffset++] = obj.materials[prim.materialIndex];
            }else{
                for(auto idx : prim.indices) OpaqueMask.push_back(idx + vboOffset); 
            }
            glm::mat4 normalMat = glm::transpose(glm::inverse(obj.nodeGlobalMatrix[prim.nodeIndex]));
            
            ddSSBOptr[m_drawDataOffset]={obj.instanceID, (unsigned)prim.morphPositions.size(), 
                                         (morphWeightsOffsets.size()>(unsigned)prim.nodeIndex) ? morphWeightsOffsets[prim.nodeIndex] : -1,
                                         m_morphPositionsOffset,
                                         (skinOffsets.size()>(unsigned)prim.nodeIndex) ?  skinOffsets[prim.skinIndex] : -1,
                                         (unsigned)prim.vertices.size(),
                                         nodeMatrixBaseOffset+prim.nodeIndex, materialDefaultSlot, normalMat
                                        };
            

            for(std::vector<glm::vec3>& morphPositions : prim.morphPositions){
                memcpy(mpSSBOptr + m_morphPositionsOffset, morphPositions.data(), morphPositions.size()* sizeof(glm::vec3));
                m_morphPositionsOffset+=morphPositions.size();
            }
            /*
            std::vector<unsigned int> ind = prim.indices;
            for(unsigned i = 0; i<ind.size();i++){
                indexPtr[m_indexCount++] = ind[i] + vboOffset;
            }*/
            
            unsigned localVertexIndex = 0;
            
            for(const Vertex3D& vertex: prim.vertices){
                minPos = glm::min(minPos, vertex.pos);
                maxPos = glm::max(maxPos, vertex.pos);
                Vertex3D currentVertex;
                currentVertex = vertex;
                currentVertex.drawID = m_drawDataOffset;
                currentVertex.vertexIndex = localVertexIndex++;
                m_Buff[vboOffset++] = currentVertex;
            }
            m_drawDataOffset++;
        }
        m_instanceCount++;
    }
    indexPtr = std::copy(OpaqueMask.begin(), OpaqueMask.end(),indexPtr);
    indexPtr = std::copy(blend.begin(), blend.end(),indexPtr);
    // After fill loop, before feedback draw:

    //std::cout << "Vertex buffer size: " << (vboOffset)*sizeof(Vertex3D) <<std::endl;
    /*
    glUnmapNamedBuffer(m_VBO);
    glUnmapNamedBuffer(m_IBO);
    glUnmapNamedBuffer(m_ModelMatricesSSBO);
    glUnmapNamedBuffer(m_MorphPositionsSSBO);
    glUnmapNamedBuffer(m_JointMatricesSSBO);
    glUnmapNamedBuffer(m_MorphWeightsSSBO);
    glUnmapNamedBuffer(m_NodeMatricesSSBO);*/
    Objects3DToRender.clear();
    
    /////////////////////
    
    /*if(firstFlush){
        std::cout << "sizeof(Vertex3D) = " << sizeof(Vertex3D) << "\n";
        std::cout << "Total VBO data = " << (float)vboOffset * sizeof(Vertex3D) / (1024*1024) << "MB\n";
        std::cout << "Total IBO data = " << (float)m_indexCount * sizeof(unsigned) / (1024*1024) << "MB\n";
        std::cout << "Total vertices: " << vboOffset << "\n";
        std::cout << "Total indices: " << m_indexCount << "\n";
        std::cout << "Ratio: " << (float)m_indexCount / vboOffset << "\n";
        firstFlush=false;
        std::cout << "Flushed Once" << std::endl;
        
    }*/
    glFlushMappedNamedBufferRange(m_VBO, 0, vboOffset * sizeof(Vertex3D));
    glFlushMappedNamedBufferRange(m_IBO, 0, (OpaqueMask.size()+blend.size()) * sizeof(unsigned int));
    glFlushMappedNamedBufferRange(m_ModelMatricesSSBO, 0, m_instanceCount * sizeof(glm::mat4));
    glFlushMappedNamedBufferRange(m_MorphPositionsSSBO, 0, m_morphPositionsOffset * sizeof(glm::vec3));
    glFlushMappedNamedBufferRange(m_JointMatricesSSBO, 0, m_jointOffset * sizeof(glm::mat4));
    glFlushMappedNamedBufferRange(m_MorphWeightsSSBO, 0, m_morphWeightsOffset * sizeof(float));
    glFlushMappedNamedBufferRange(m_NodeMatricesSSBO, 0, m_nodeMatrixOffset * sizeof(glm::mat4));
    glFlushMappedNamedBufferRange(m_MaterialsSSBO, 0, m_materialOffset * sizeof(Material));
    glFlushMappedNamedBufferRange(m_DrawDataSSBO, 0, m_drawDataOffset * sizeof(DrawData));

    // Compute light space matrix
    

    
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ModelMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_MorphPositionsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_JointMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_MorphWeightsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_NodeMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_MaterialsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_DrawDataSSBO);
    
    
    

    glm::mat4 viewMat = Engine::getInstance().camera3d.getMatrix();
    glm::vec3 camForward = Engine::getInstance().camera3d.getForward();


    glm::vec3 lightDir = glm::normalize(glm::mat3(viewMat) * camForward);
    glm::vec3 lightColor =  glm::vec3(1.f, 1.f, 1.f);

    
    glm::vec3 lightPos = glm::vec3(viewMat * glm::vec4(Engine::getInstance().camera3d.getPosition(), 1.0f));

    /*glm::mat4 lightView = glm::lookAt(lightPos, sceneCenter, 
                                    glm::vec3(0.0f, -1.0f, 0.0f));
    glm::mat4 lightProj = glm::ortho(-sceneRadius, sceneRadius,
                                    -sceneRadius, sceneRadius,
                                    0.1f, sceneRadius * 4.0f);
    */

    //printf("%f %f %f\n", camForward.x, camForward.y, camForward.z);
    glUniformMatrix4fv(m_uProjMatrix, 1,GL_FALSE,Engine::getInstance().getProjMatrix());
    glUniformMatrix4fv(m_uVwMatrix,1,GL_FALSE, glm::value_ptr(viewMat));
    glUniform3fv(m_uLightPos,   1, glm::value_ptr(lightPos));
    glUniform3fv(m_uLightDir,   1, glm::value_ptr(lightDir));
    glUniform3fv(m_uViewPos,    1, glm::value_ptr(Engine::getInstance().camera3d.getPosition()));
    glUniform3fv(m_uLightColor, 1, glm::value_ptr(lightColor));
    glUniform1f(m_uLightIntensity, 5.f);
    glUniform1f(m_uAmbientStrength, 0.01f);
    glUniform1i(m_uLightMode, 1);
    
    glBindVertexArray(m_VAO);
    /*if (m_timerQueryReady) {
        GLuint64 elapsed;
        glGetQueryObjectui64v(m_timerQuery[1 - m_timerQueryIndex], 
                            GL_QUERY_RESULT, &elapsed);
        static int fc = 0;
        //if (fc++ % 60 == 0)
        //    std::cout << "GPU draw time: " << elapsed / 1000000.0f << "ms\n";
    }*/
    //glBeginQuery(GL_TIME_ELAPSED, m_timerQuery[m_timerQueryIndex]);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);
    glDrawElements(GL_TRIANGLES, OpaqueMask.size(), GL_UNSIGNED_INT, NULL);
    
    //glDepthMask(GL_FALSE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawElements(GL_TRIANGLES, blend.size(), GL_UNSIGNED_INT, (void*)(OpaqueMask.size()*sizeof(unsigned)));
    
    
    m_fence= glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glBindVertexArray(0);
    
    glUseProgram(0);
    
}
