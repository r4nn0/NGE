#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <chrono>
#include "glm/glm.hpp"

struct TextureInfo {
    GLuint textureID;
    uint64_t bindlessHandle;
    int width;
    int height;
    int mipLevels;
    size_t memorySize;
    std::chrono::steady_clock::time_point lastAccess;
    int refCount;
    bool isResident;
};

class TextureManager {
public:
    static TextureManager& getInstance();
    
    // Load texture from file path
    int loadTexture(const char* path, bool generateMips = true);
    
    // Load texture from memory (for embedded glTF textures)
    int loadTextureFromMemory(const unsigned char* data, size_t size, const std::string& key, bool generateMips = true, 
                             int width = 0, int height = 0, int components = 0);
    
    // Get bindless handle for a texture
    uint64_t getTextureHandle(int textureID);
    
    // Get GL texture ID for traditional binding
    GLuint getGLTextureID(int textureID);
    
    // Mark texture as used (for LRU)
    void touchTexture(int textureID);
    
    // Increase reference count
    void addRef(int textureID);
    
    // Decrease reference count
    void releaseRef(int textureID);
    
    // Cleanup unused textures
    void cleanupUnusedTextures();
    
    // Get texture info
    const TextureInfo* getTextureInfo(int textureID) const;
    
    // Set memory budget (in bytes)
    void setMemoryBudget(size_t budget);
    
    // Get current memory usage
    size_t getCurrentMemoryUsage() const;
    
    // Shutdown
    void shutdown();
    int getTextureCount();
private:
    TextureManager();
    ~TextureManager();
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    
    // Internal texture creation
    int createTexture(GLuint glTexture, int width, int height, int mipLevels, size_t memorySize);
    
    // Generate mipmaps
    void generateMipmaps(GLuint texture, int width, int height);
    
    // Calculate memory size
    size_t calculateMemorySize(int width, int height, int mipLevels);
    
    // Evict textures to fit budget
    void evictTexturesIfNeeded();
    
    std::unordered_map<int, TextureInfo> m_textures;
    std::unordered_map<std::string, int> m_pathToID;
    int m_nextID;
    
    size_t m_memoryBudget;
    size_t m_currentMemoryUsage;
};

#endif // TEXTURE_MANAGER_H
