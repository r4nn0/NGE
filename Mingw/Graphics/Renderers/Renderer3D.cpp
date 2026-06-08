#include "Renderer3D.h"
#include <glm/gtx/string_cast.hpp>
#define MEGABYTE 1048576
#define VERTEX_SIZE3D sizeof(Vertex3D)
#define BUFFER_SIZE3D 130*MEGABYTE
#define INDICES_SIZE3D BUFFER_SIZE3D/10
#define MODEL_MATRICES_SSBO_SIZE3D 5*MEGABYTE
#define MORPH_POSITIONS_SSBO_SIZE3D 5*MEGABYTE
#define JOINT_MATRICES_SSBO_SIZE3D 5*MEGABYTE
#define MORPH_WEIGHTS_SSBO_SIZE3D 5*MEGABYTE
#define NODE_MATRICES_SSBO_SIZE3D 5*MEGABYTE
#define MATERIAL_SSBO_SIZE_3D 5*MEGABYTE

/**
 * @brief Initialize batch renderer to pass correct values to shaders
 * 
 */
Renderer3D::Renderer3D() : vboOffset(0), m_indexCount(0), m_instanceCount(0),m_morphPositionsOffset(0), m_jointOffset(0), m_morphWeightsOffset(0), m_nodeMatrixOffset(0), m_materialOffset(1), m_drawDataOffset(0) , m_fence(nullptr), window_size(Engine::getWindowSize()){
    glGenQueries(2, m_timerQuery);
    glCreateVertexArrays(1, &m_appSurface);

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

    glVertexArrayVertexBuffer(m_appSurface,0,m_VBO,0, VERTEX_SIZE3D);
    glVertexArrayElementBuffer(m_appSurface, m_IBO);



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
        glEnableVertexArrayAttrib(m_appSurface,i);
        glVertexArrayAttribBinding(m_appSurface,i,0);
    }
    
    glVertexArrayAttribFormat(m_appSurface, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, pos));
    glVertexArrayAttribFormat(m_appSurface, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, texCoords));
    glVertexArrayAttribFormat(m_appSurface, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex3D, color));
    glVertexArrayAttribFormat(m_appSurface, 3, 2, GL_UNSIGNED_SHORT, GL_TRUE, offsetof(Vertex3D, normal));

    glVertexArrayAttribIFormat(m_appSurface, 4, 4, GL_UNSIGNED_BYTE, offsetof(Vertex3D, joints));
    glVertexArrayAttribFormat(m_appSurface,  5, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex3D, weights));

    glVertexArrayAttribIFormat(m_appSurface, 6, 1, GL_UNSIGNED_INT, offsetof(Vertex3D, vertexIndex));
    glVertexArrayAttribIFormat(m_appSurface, 7, 1, GL_UNSIGNED_INT, offsetof(Vertex3D, drawID));

    m_Shader=Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/shader3D.vs").c_str(),
                                  Engine::LoadShaderFromFile("./Graphics/Shaders/shader3D.fs").c_str());
    
    m_uProjMatrix      = glGetUniformLocation(m_Shader, "proj_matrix");
    m_uVwMatrix        = glGetUniformLocation(m_Shader, "vw_matrix");
    m_uLightDir        = glGetUniformLocation(m_Shader, "lightDir");
    m_uViewPos         = glGetUniformLocation(m_Shader, "viewPos");
    m_uLightPos        = glGetUniformLocation(m_Shader, "lightPos");
    m_uLightColor      = glGetUniformLocation(m_Shader, "lightColor");
    m_uLightInnerCutoff = glGetUniformLocation(m_Shader, "lightCutoff");
    m_uLightOuterCutoff = glGetUniformLocation(m_Shader, "lightOuterCutoff");

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
    
}

/**
 * @brief Start rendering objects added to the renderer
 * 
 */
void Renderer3D::Render(){
    
    window_size = Engine::getWindowSize();
    static bool firstFlush = true;

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
        return;
    }
    
    if (m_fence) {
        glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, GL_TIMEOUT_IGNORED);
        glDeleteSync(m_fence);
        m_fence = nullptr;
    }
    
    Material defaultMat{};
    materialSSBOptr[0] = defaultMat;
    
    for(Object3D& obj:ObjectsToRender){
        if(obj.wasRendered) continue;
        else obj.wasRendered=true;
        firstFlush=true;
        std::map<int, int> skinOffsets;
        std::vector<int> morphWeightsOffsets;
        //GLsizei localJointOffset=0;
        mmSSBOptr[m_instanceCount] = obj.modelMatrix;
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
                //printf("Materials Size: %d, Material Index: %d\n", obj.materials.size(), prim.materialIndex);
                materialDefaultSlot = m_materialOffset;
                materialSSBOptr[m_materialOffset++] = obj.materials[prim.materialIndex];
            }
            glm::mat4 normalMat = glm::transpose(glm::inverse(obj.modelMatrix*obj.nodeGlobalMatrix[prim.nodeIndex]));
            ddSSBOptr[m_drawDataOffset]={m_instanceCount, prim.morphPositions.size(), 
                                         morphWeightsOffsets[prim.nodeIndex], m_morphPositionsOffset,
                                         skinOffsets[prim.skinIndex], prim.vertices.size(),
                                         nodeMatrixBaseOffset+prim.nodeIndex, materialDefaultSlot, normalMat
                                        };


            for(std::vector<glm::vec3>& morphPositions : prim.morphPositions){
                memcpy(mpSSBOptr + m_morphPositionsOffset, morphPositions.data(), morphPositions.size()* sizeof(glm::vec3));
                m_morphPositionsOffset+=morphPositions.size();
            }
            std::vector<unsigned int> ind = prim.indices;
            for(unsigned i = 0; i<ind.size();i++){
                indexPtr[m_indexCount++] = ind[i] + vboOffset;
            }
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
    ObjectsToRender.clear();
    
    /////////////////////
    
    if(firstFlush){
        std::cout << "sizeof(Vertex3D) = " << sizeof(Vertex3D) << "\n";
        std::cout << "Total VBO data = " << (float)vboOffset * sizeof(Vertex3D) / (1024*1024) << "MB\n";
        std::cout << "Total IBO data = " << (float)m_indexCount * sizeof(unsigned) / (1024*1024) << "MB\n";
        std::cout << "Total vertices: " << vboOffset << "\n";
        std::cout << "Total indices: " << m_indexCount << "\n";
        std::cout << "Ratio: " << (float)m_indexCount / vboOffset << "\n";
        firstFlush=false;
        std::cout << "Flushed Once" << std::endl;
        glFlushMappedNamedBufferRange(m_VBO, 0, vboOffset * sizeof(Vertex3D));
        glFlushMappedNamedBufferRange(m_IBO, 0, m_indexCount * sizeof(unsigned int));
        glFlushMappedNamedBufferRange(m_ModelMatricesSSBO, 0, m_instanceCount * sizeof(glm::mat4));
        glFlushMappedNamedBufferRange(m_MorphPositionsSSBO, 0, m_morphPositionsOffset * sizeof(glm::vec3));
        glFlushMappedNamedBufferRange(m_JointMatricesSSBO, 0, m_jointOffset * sizeof(glm::mat4));
        glFlushMappedNamedBufferRange(m_MorphWeightsSSBO, 0, m_morphWeightsOffset * sizeof(float));
        glFlushMappedNamedBufferRange(m_NodeMatricesSSBO, 0, m_nodeMatrixOffset * sizeof(glm::mat4));
        glFlushMappedNamedBufferRange(m_MaterialsSSBO, 0, m_materialOffset * sizeof(Material));
        glFlushMappedNamedBufferRange(m_DrawDataSSBO, 0, m_drawDataOffset * sizeof(DrawData));
    }

    // Compute light space matrix
    glm::vec3 sceneCenter = (minPos+maxPos)*0.5f;
    glm::vec3 size = maxPos-minPos;
    float sceneRadius     = glm::max(size.x, glm::max(size.y, size.z));
    //float angle = glfwGetTime() * 0.1f;
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, 0.8f, 0.0f));
    glm::vec3 lightPos = sceneCenter - lightDir * sceneRadius * 2.0f;

    glm::mat4 lightView = glm::lookAt(lightPos, sceneCenter, 
                                    glm::vec3(0.0f, -1.0f, 0.0f));

    glm::mat4 lightProj = glm::ortho(-sceneRadius, sceneRadius,
                                    -sceneRadius, sceneRadius,
                                    0.1f, sceneRadius * 4.0f);

    
    glUseProgram(m_Shader);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ModelMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_MorphPositionsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_JointMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_MorphWeightsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_NodeMatricesSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_MaterialsSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, m_DrawDataSSBO);
    
    
    glUniformMatrix4fv(m_uProjMatrix, 1,GL_FALSE,Engine::getProjMatrix());
    glUniformMatrix4fv(m_uVwMatrix,1,GL_FALSE, Engine::getViewMatrix());
    glm::mat4 viewMat = Engine::camera3d.getMatrix();
    glm::vec3 camForward = -glm::vec3(viewMat[0][2],viewMat[1][2],viewMat[2][2]);

     
    glm::vec3 lighBrightness =  glm::vec3(0.1f);
    glUniform3f(m_uLightPos, 0, 0, 0);
    glUniform3f(m_uLightDir, lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(m_uViewPos, Engine::camera3d.getPosition().x, Engine::camera3d.getPosition().y, Engine::camera3d.getPosition().z);
    glUniform1f(m_uLightInnerCutoff, 0.0f);
    glUniform1f(m_uLightOuterCutoff, 0.0f);
    glUniform3f(m_uLightColor, lighBrightness.x, lighBrightness.y, lighBrightness.z); 
    
    
    glBindVertexArray(m_appSurface);
    /*if (m_timerQueryReady) {
        GLuint64 elapsed;
        glGetQueryObjectui64v(m_timerQuery[1 - m_timerQueryIndex], 
                            GL_QUERY_RESULT, &elapsed);
        static int fc = 0;
        //if (fc++ % 60 == 0)
        //    std::cout << "GPU draw time: " << elapsed / 1000000.0f << "ms\n";
    }*/
    //glBeginQuery(GL_TIME_ELAPSED, m_timerQuery[m_timerQueryIndex]);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, NULL);
    
    
    m_fence= glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glBindVertexArray(0);
    
    glUseProgram(0);
    
}
