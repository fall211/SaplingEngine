//
//
//
//

#include "AudioEngine.hpp"
#include "Seedbank/AssetManager.hpp"

AudioEngine* AudioEngine::Instance = nullptr;


AudioEngine::AudioEngine() : m_system(nullptr), m_studioSystem(nullptr) {
    initialize();
}

AudioEngine::~AudioEngine() {
    cleanup();
}

void AudioEngine::initialize() {
    // Initialize FMOD Studio System
    FMOD::Studio::System::create(&m_studioSystem);
    m_studioSystem->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
    
    // Get core system
    m_studioSystem->getCoreSystem(&m_system);
}

void AudioEngine::update() {
    if (m_studioSystem) {
        m_studioSystem->update();
    }
}

void AudioEngine::cleanup() {
    // Stop all channels
    for (auto& channel : m_channels) {
        if (channel.second) {
            channel.second->stop();
        }
    }
    m_channels.clear();
    
    // Release FMOD systems
    if (m_studioSystem) {
        m_studioSystem->release();
        free(m_studioSystem);
    }
    if (m_system) {
        m_system->release();
        free(m_system);
    }
}

void AudioEngine::playSound(const std::string& name, float volume) {
    FMOD::Sound* sound = AssetManager::getInstance()->getSound(name);
    if (sound) {
        FMOD::Channel* channel = nullptr;
        m_system->playSound(sound, nullptr, false, &channel);
        if (channel) {
            channel->setVolume(volume);
            m_channels[name] = channel;
        }
    }
}

void AudioEngine::stopSound(const std::string& name) {
    auto it = m_channels.find(name);
    if (it != m_channels.end() && it->second) {
        it->second->stop();
    }
}

void AudioEngine::setPitch(const std::string& name, float pitch) {
    auto it = m_channels.find(name);
    if (it != m_channels.end() && it->second) {
        it->second->setPitch(pitch);
    }
}

void AudioEngine::setVolume(const std::string& name, float volume) {
    auto it = m_channels.find(name);
    if (it != m_channels.end() && it->second) {
        it->second->setVolume(volume);
    }
}

bool AudioEngine::isPlaying(const std::string& name) {
    auto it = m_channels.find(name);
    if (it != m_channels.end() && it->second) {
        bool playing = false;
        it->second->isPlaying(&playing);
        return playing;
    }
    return false;
}