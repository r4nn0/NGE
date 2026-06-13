#include "TextureManager.h"
#include "stb_image.h"
#include <iostream>
#include <algorithm>

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

TextureManager::TextureManager() 
    : m_nextID(0)
    , m_memoryBudget(2048 * 1024 * 1024) // 512MB default
    , m_currentMemoryUsage(0) {
}

TextureManager::~TextureManager() {
    shutdown();
}

int TextureManager::loadTexture(const char* path, bool generateMips) {
    // Check if already loaded
    auto it = m_pathToID.find(path);
    if (it != m_pathToID.end()) {
        touchTexture(it->second);
        addRef(it->second);
        return it->second;
    }
    
    // Load image using stb_image
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return -1;
    }
    
    // Create OpenGL texture
    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    // Calculate mip levels
    int mipLevels = generateMips ? (int)std::floor(std::log2(std::max(width, height))) + 1 : 1;
    
    // Allocate storage
    glTextureStorage2D(texture, mipLevels, GL_RGBA8, width, height);
    
    // Upload base level
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    // Generate mipmaps if requested
    if (generateMips && mipLevels > 1) {
        glGenerateTextureMipmap(texture);
    }
    
    // Set texture parameters
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, generateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
    
    // Calculate memory size
    size_t memorySize = calculateMemorySize(width, height, mipLevels);
    
    // Create texture entry
    int textureID = createTexture(texture, width, height, mipLevels, memorySize);
    
    if (textureID >= 0) {
        m_pathToID[path] = textureID;
    }
    
    return textureID;
}

int TextureManager::loadTextureFromMemory(const unsigned char* data, size_t size, const std::string& key, bool generateMips, int width, int height, int components) {
    // Check if already loaded
    auto it = m_pathToID.find(key);
    if (it != m_pathToID.end()) {
        touchTexture(it->second);
        addRef(it->second);
        return it->second;
    }
    
    if (!data || size == 0) {
        std::cerr << "Failed to load texture from memory: " << key << " - null data or zero size" << std::endl;
        return -1;
    }
    
    unsigned char* imageData = nullptr;
    bool shouldFree = false;
    
    // If width/height/components are provided, use the pre-decoded data from tinygltf
    if (width > 0 && height > 0 && components > 0) {
        imageData = const_cast<unsigned char*>(data);
    } else {
        // Try to load with stb_image first (for compressed formats like PNG, JPG)
        int channels;
        stbi_set_flip_vertically_on_load(true);
        imageData = stbi_load_from_memory(data, size, &width, &height, &channels, 4);
        
        if (imageData) {
            shouldFree = true;
        } else {
            std::cerr << "Failed to load texture from memory: " << key << " - stb_image error: " << stbi_failure_reason() << std::endl;
            return -1;
        }
    }
    
    // Determine GL format based on components
    GLenum format = GL_RGBA;
    if (components == 1) format = GL_RED;
    else if (components == 2) format = GL_RG;
    else if (components == 3) format = GL_RGB;
    else if (components == 4) format = GL_RGBA;
    
    // Create OpenGL texture
    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    
    // Calculate mip levels
    int mipLevels = generateMips ? (int)std::floor(std::log2(std::max(width, height))) + 1 : 1;
    
    // Set texture parameters BEFORE allocating storage (required for completeness)
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, generateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Allocate storage with appropriate internal format
    GLenum internalFormat = (components == 4) ? GL_RGBA8 : (components == 3) ? GL_RGB8 : (components == 2) ? GL_RG8 : GL_R8;
    glTextureStorage2D(texture, mipLevels, internalFormat, width, height);
    
    // Upload base level
    glTextureSubImage2D(texture, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, imageData);
    
    // Generate mipmaps if requested
    if (generateMips && mipLevels > 1) {
        glGenerateTextureMipmap(texture);
    }
    
    // Free if we allocated with stb_image
    if (shouldFree) {
        stbi_image_free(imageData);
    }
    
    // Calculate memory size
    size_t memorySize = calculateMemorySize(width, height, mipLevels);
    
    // Create texture entry
    int textureID = createTexture(texture, width, height, mipLevels, memorySize);
    
    if (textureID >= 0) {
        m_pathToID[key] = textureID;
    }
    
    return textureID;
}

int TextureManager::createTexture(GLuint glTexture, int width, int height, int mipLevels, size_t memorySize) {
    // Check if we need to evict textures
    evictTexturesIfNeeded();
    
    // Check if texture is complete before creating bindless handle
    if (!glIsTexture(glTexture)) {
        std::cerr << "Texture is not valid" << std::endl;
        glDeleteTextures(1, &glTexture);
        return -1;
    }
    
    
    // Create bindless handle
    uint64_t handle = glGetTextureHandleARB(glTexture);
    
    if (handle == 0) {
        std::cerr << "Failed to get texture handle" << std::endl;
        glDeleteTextures(1, &glTexture);
        return -1;
    }
    // Make handle resident
    glMakeTextureHandleResidentARB(handle);
    
    // Check for OpenGL errors immediately after making resident
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after making handle resident: " << err << std::endl;
        glMakeTextureHandleNonResidentARB(handle);
        glDeleteTextures(1, &glTexture);
        return -1;
    }
    
    // Create texture info
    TextureInfo info;
    info.textureID = glTexture;
    info.bindlessHandle = handle;
    info.width = width;
    info.height = height;
    info.mipLevels = mipLevels;
    info.memorySize = memorySize;
    info.lastAccess = std::chrono::steady_clock::now();
    info.refCount = 1;
    info.isResident = true;
    
    int textureID = m_nextID++;
    m_textures[textureID] = info;
    m_currentMemoryUsage += memorySize;
    
    std::cout << "Texture created with ID: " << textureID << ", total memory usage: " << (m_currentMemoryUsage / 1024 / 1024) << " MB" << std::endl;
    
    return textureID;
}

uint64_t TextureManager::getTextureHandle(int textureID) {
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        touchTexture(textureID);
        return it->second.bindlessHandle;
    }
    return 0;
}

GLuint TextureManager::getGLTextureID(int textureID) {
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        touchTexture(textureID);
        return it->second.textureID;
    }
    return 0;
}

void TextureManager::touchTexture(int textureID) {
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        it->second.lastAccess = std::chrono::steady_clock::now();
    }
}

void TextureManager::addRef(int textureID) {
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        it->second.refCount++;
    }
}

void TextureManager::releaseRef(int textureID) {
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        it->second.refCount--;
        if (it->second.refCount <= 0) {
            // Texture can be evicted
        }
    }
}

void TextureManager::cleanupUnusedTextures() {
    std::vector<int> toRemove;
    
    for (auto& pair : m_textures) {
        if (pair.second.refCount <= 0) {
            toRemove.push_back(pair.first);
        }
    }
    
    for (int id : toRemove) {
        auto it = m_textures.find(id);
        if (it != m_textures.end()) {
            // Make handle non-resident
            if (it->second.isResident) {
                glMakeTextureHandleNonResidentARB(it->second.bindlessHandle);
            }
            glDeleteTextures(1, &it->second.textureID);
            m_currentMemoryUsage -= it->second.memorySize;
            m_textures.erase(it);
        }
    }
}

const TextureInfo* TextureManager::getTextureInfo(int textureID) const {
    auto it = m_textures.find(textureID);
    if (it != m_textures.end()) {
        return &it->second;
    }
    return nullptr;
}

void TextureManager::setMemoryBudget(size_t budget) {
    m_memoryBudget = budget;
    evictTexturesIfNeeded();
}

size_t TextureManager::getCurrentMemoryUsage() const {
    return m_currentMemoryUsage;
}

void TextureManager::shutdown() {
    // Cleanup all textures
    for (auto& pair : m_textures) {
        if (pair.second.isResident) {
            glMakeTextureHandleNonResidentARB(pair.second.bindlessHandle);
        }
        glDeleteTextures(1, &pair.second.textureID);
    }
    
    m_textures.clear();
    m_pathToID.clear();
    m_currentMemoryUsage = 0;
}

size_t TextureManager::calculateMemorySize(int width, int height, int mipLevels) {
    size_t size = 0;
    int w = width;
    int h = height;
    for (int i = 0; i < mipLevels; i++) {
        size += w * h * 4; // 4 bytes per pixel (RGBA8)
        w = std::max(1, w / 2);
        h = std::max(1, h / 2);
    }
    return size;
}

void TextureManager::evictTexturesIfNeeded() {
    while (m_currentMemoryUsage > m_memoryBudget && !m_textures.empty()) {
        // Find least recently used texture with refCount <= 0
        int oldestID = -1;
        std::chrono::steady_clock::time_point oldestTime;
        
        for (auto& pair : m_textures) {
            if (pair.second.refCount <= 0) {
                if (oldestID < 0 || pair.second.lastAccess < oldestTime) {
                    oldestID = pair.first;
                    oldestTime = pair.second.lastAccess;
                }
            }
        }
        
        if (oldestID < 0) {
            // No textures to evict
            break;
        }
        
        // Evict the texture
        auto it = m_textures.find(oldestID);
        if (it != m_textures.end()) {
            if (it->second.isResident) {
                glMakeTextureHandleNonResidentARB(it->second.bindlessHandle);
            }
            glDeleteTextures(1, &it->second.textureID);
            m_currentMemoryUsage -= it->second.memorySize;
            
            // Remove from path map
            for (auto pit = m_pathToID.begin(); pit != m_pathToID.end(); ) {
                if (pit->second == oldestID) {
                    pit = m_pathToID.erase(pit);
                } else {
                    ++pit;
                }
            }
            
            m_textures.erase(it);
        }
    }
}
int TextureManager::getTextureCount(){
    return m_nextID;
}