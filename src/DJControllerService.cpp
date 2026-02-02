#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    if (cache.contains(track.get_title())) {
        cache.get(track.get_title());
        return 1;
    }
    PointerWrapper<AudioTrack> p = track.clone();
    if (!p) {
        std::cerr << "[ERROR] Track: "<< track.get_title() << " failed to clone" << std::endl;
        return 2; //error code
    } else {
        p->load();
        p->analyze_beatgrid();
        bool result = cache.put(std::move(p));
        return result ? -1 : 0;
    }
}

void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}

void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}


AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    return cache.get(track_title);
}
