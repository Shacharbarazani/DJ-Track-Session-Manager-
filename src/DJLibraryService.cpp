#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include "PointerWrapper.h" 
#include <iostream>
#include <memory>
#include <algorithm>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist), library() {}


DJLibraryService::~DJLibraryService(){
    for (AudioTrack* track : library) {
        delete track;
    }
    library.clear();
}

DJLibraryService::DJLibraryService(const DJLibraryService& other): playlist(other.playlist), library() {
    for (AudioTrack* track : other.library) {
        if (track) {
            PointerWrapper<AudioTrack> clone = track->clone();
            if (clone) {
                library.push_back(clone.release());
            }
        }
    }
}

DJLibraryService& DJLibraryService:: operator=(const DJLibraryService& other){
    if (this!=&other){
        //clean:
        for (AudioTrack* track : library) {
            delete track;
        }
        library.clear();
        
        //copy:
        playlist = other.playlist;
        for (AudioTrack* track : other.library) {
            if (track) {
                PointerWrapper<AudioTrack> clone = track->clone();
                if (clone) {
                    library.push_back(clone.release());
                }
            }
        }
    }
    return *this;
}

/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    for (const SessionConfig::TrackInfo& track : library_tracks) {
        if (track.type=="MP3") {
            AudioTrack* new_track = new MP3Track(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
            library.push_back(new_track);
        }
        else if (track.type == "WAV") {
            AudioTrack* new_track = new WAVTrack(track.title, track.artists, track.duration_seconds, track.bpm, track.extra_param1, track.extra_param2);
            library.push_back(new_track);
        }
    }
    std::cout << "[INFO] Track library built: " << library.size() << " tracks loaded" << std::endl;
 }

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}


AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    return (playlist.find_track(track_title));
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    std::cout << "[INFO] Loading playlist: " <<playlist_name<< std::endl;
    Playlist new_playlist = Playlist(playlist_name);
    int limit = library.size();
    for (const int& index : track_indices){
        if (index > limit || index < 1) {
            std::cout << "[WARNING] Invalid track index: " <<index<< std::endl;
            continue; //move to the next index
        }
        if (!library[index-1]) {
            std::cout << "[ERROR] Library slot " << index-1 << " is null" << std::endl;
            continue;
        }
        PointerWrapper<AudioTrack> clone = library[index-1]->clone();
        if (!clone){
            std::cout << "[ERROR] Track: " << library[index-1]->get_title() << " failed to clone" << std::endl;
            continue; 
        }
        clone->load();
        clone->analyze_beatgrid();
        new_playlist.add_track(clone.release());
    }
    playlist= new_playlist;
    std::cout << "[INFO] Playlist loaded: " <<playlist_name<< " (" <<track_indices.size()<< " tracks)" << std::endl;
}

/**
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    std::vector<std::string> titles;
    std::vector<AudioTrack*> tracks = playlist.getTracks();
    if (tracks.size() > 0) {
        for (AudioTrack* track : tracks) {
            if(track){
                titles.push_back(track->get_title());
            }
        }
    }
    std::reverse(titles.begin(), titles.end());
    return titles;
}
