#include "SkyboxRenderer.h"

Skybox::Skybox(){
    skyboxVertices = {
        -1,  1, -1,  -1, -1, -1,   1, -1, -1,   1, -1, -1,   1,  1, -1,  -1,  1, -1,
        -1, -1,  1,  -1, -1, -1,  -1,  1, -1,  -1,  1, -1,  -1,  1,  1,  -1, -1,  1,
        1, -1, -1,   1, -1,  1,   1,  1,  1,   1,  1,  1,   1,  1, -1,   1, -1, -1,
        -1, -1,  1,  -1,  1,  1,   1,  1,  1,   1,  1,  1,   1, -1,  1,  -1, -1,  1,
        -1,  1, -1,   1,  1, -1,   1,  1,  1,   1,  1,  1,  -1,  1,  1,  -1,  1, -1,
        -1, -1, -1,  -1, -1,  1,   1, -1, -1,   1, -1, -1,  -1, -1,  1,   1, -1,  1
    };
    
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    
    std::vector<std::string> faces = {
        "SkyboxTest/px.png",  // +X right
        "SkyboxTest/nx.png",  // -X left
        "SkyboxTest/ny.png",  // +Y top
        "SkyboxTest/py.png",  // -Y bottom
        "SkyboxTest/pz.png",  // +Z front
        "SkyboxTest/nz.png"   // -Z back
    };
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    for (int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 3);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height,
                0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            std::cerr << "Failed to load cubemap face: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    stbi_set_flip_vertically_on_load(false);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenVertexArrays(1, &m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size()*sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_Shader=Engine::CreateShader(Engine::LoadShaderFromFile("./Graphics/Shaders/skyboxShader.vs").c_str(),
                                  Engine::LoadShaderFromFile("./Graphics/Shaders/skyboxShader.fs").c_str());
}

void Skybox::Render(){
    glDepthFunc(GL_LEQUAL);

    glUseProgram(m_Shader);
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "view"), 1, GL_FALSE, Engine::getViewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "projection"), 1, GL_FALSE, Engine::getProjMatrix());
    glUniform1i(glGetUniformLocation(m_Shader, "skybox"), 0);

    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}