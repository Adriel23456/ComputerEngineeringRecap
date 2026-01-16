// ============================================================================
// File: src/core/resources/TextureCache.cpp
// ============================================================================

/**
 * @file TextureCache.cpp
 * @brief Implementation of TextureCache singleton.
 */

#include "core/resources/TextureCache.h"
#include <iostream>

TextureCache& TextureCache::instance() {
    static TextureCache instance;
    return instance;
}

std::shared_ptr<sf::Texture> TextureCache::get(const std::string& fullPath) {
    // Check if texture is already cached and still alive
    auto it = m_cache.find(fullPath);
    if (it != m_cache.end()) {
        if (auto existingTexture = it->second.lock()) {
            return existingTexture;
        }
    }

    // Load new texture
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(fullPath)) {
        std::cout << "[TextureCache] Failed to load texture: " << fullPath << "\n";
        return nullptr;
    }

    texture->setSmooth(true);
    m_cache[fullPath] = texture;

    return texture;
}

void TextureCache::clear() {
    m_cache.clear();
}