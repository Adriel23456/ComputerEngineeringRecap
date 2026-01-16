// ============================================================================
// File: include/core/resources/TextureCache.h
// ============================================================================

#pragma once

/**
 * @file TextureCache.h
 * @brief Texture resource caching system.
 *
 * Provides centralized texture loading and caching using weak
 * pointers to allow automatic cleanup when textures are no longer used.
 *
 * @note Follows:
 *   - SRP: Only manages texture caching
 *   - Singleton pattern for global access
 */

#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <string>
#include <unordered_map>

 /**
  * @class TextureCache
  * @brief Singleton texture cache manager.
  *
  * Caches loaded textures using weak pointers, allowing automatic
  * cleanup when all strong references are released.
  */
class TextureCache {
public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the texture cache instance.
     */
    static TextureCache& instance();

    /**
     * @brief Gets or loads a texture from the cache.
     *
     * If the texture is already cached and still in use, returns
     * the existing shared pointer. Otherwise loads from disk.
     *
     * @param fullPath Complete path to the texture file.
     * @return Shared pointer to the texture, or nullptr on failure.
     */
    std::shared_ptr<sf::Texture> get(const std::string& fullPath);

    /**
     * @brief Clears all cached texture references.
     *
     * Note: This only clears the cache entries. Textures still
     * held by other shared_ptrs will remain valid.
     */
    void clear();

private:
    TextureCache() = default;

    // Prevent copying (singleton)
    TextureCache(const TextureCache&) = delete;
    TextureCache& operator=(const TextureCache&) = delete;

    /// Cache storage: path -> weak reference to texture
    std::unordered_map<std::string, std::weak_ptr<sf::Texture>> m_cache;
};