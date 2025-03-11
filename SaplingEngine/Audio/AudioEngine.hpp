//
// SaplingEngine Audio Engine
// Made using FMOD Studio by Firelight Technologies Pty Ltd.
//


#pragma once

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include <unordered_map>
#include <string>

class AssetManager;

class AudioEngine
{
    private:
        static AudioEngine* Instance;
    
        FMOD::System* m_system;
        FMOD::Studio::System* m_studioSystem;
        std::unordered_map<std::string, FMOD::Channel*> m_channels;
        
        AudioEngine();
        ~AudioEngine();
        
    public:
        static AudioEngine* getInstance()
        {
            if (!Instance)
            {
                Instance = new AudioEngine();
            }
            return Instance;
        }
        FMOD::System* getSystem()
        {
            return m_system;
        }
        
        void initialize();
        void update();
        void cleanup();
        
        void playSound(const std::string& soundName, float volume = 1.0f);
        void stopSound(const std::string& soundName);
        void setPitch(const std::string& soundName, float pitch);
        void setVolume(const std::string& soundName, float volume);
        bool isPlaying(const std::string& soundName);
};