#include "MixingEngineService.h"
#include <iostream>
#include <memory>



MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    std::cout << "[MixingEngineService] Initialized with 2 empty decks." << std::endl;
}


MixingEngineService::~MixingEngineService() {
    std::cout<< "[MixingEngineService] Cleaning up decks..."<<std::endl;
    for (AudioTrack*& deck : decks) {
        delete deck;   
        deck = nullptr;
    }
}

MixingEngineService::MixingEngineService(const MixingEngineService& other): decks{nullptr, nullptr}, active_deck(other.active_deck), auto_sync(other.auto_sync), bpm_tolerance(other.bpm_tolerance){
    if (other.decks[0]) {
    decks[0] = other.decks[0]->clone().release();
    }
    if (other.decks[1]) {
    decks[1] = other.decks[1]->clone().release();
    }
}

MixingEngineService& MixingEngineService:: operator=(const MixingEngineService& other){
    if (this != &other) {
    for (AudioTrack*& deck : decks) {
        delete deck;   
        deck = nullptr;
    }

    //copy:
    active_deck = other.active_deck;
    auto_sync = other.auto_sync;
    bpm_tolerance = other.bpm_tolerance;

    if (other.decks[0])
    decks[0] = other.decks[0]->clone().release();
    if (other.decks[1])
    decks[1] = other.decks[1]->clone().release();
}

    return *this;
}



/**
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    std::cout << "\n=== Loading Track to Deck ===" << std::endl;
    PointerWrapper<AudioTrack> clone = track.clone();
    if (!clone){
        std::cout << "[ERROR] Track: " << track.get_title() << " failed to clone" << std::endl;
        return -1;
    }
    size_t target = 1-active_deck;
    if (!decks[0] && !decks[1]) { //first track
        std::cout << "[Deck Switch] Target deck: " << target << std::endl;
        clone->load();
        clone->analyze_beatgrid();
        decks[0] = clone.release();
        active_deck = 0;
        std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck 0" << std::endl;
        std::cout << "[Active Deck] Switched to deck " << active_deck << std::endl;
        return 0; 
    }
    std::cout << "[Deck Switch] Target deck: " << target << std::endl;
    if (decks[target]){
        delete decks[target];
        decks[target] = nullptr;
    }
    clone->load();
    clone->analyze_beatgrid();
    if (auto_sync && decks[active_deck]) {
        if (!can_mix_tracks(clone)) {
            sync_bpm(clone);
        }
    }
    decks[target] = clone.release();
    std::cout << "[Load Complete] '" << track.get_title() << "' is now loaded on deck " << target << std::endl;
    active_deck = target;
    std::cout << "[Active Deck] Switched to deck " << target << std::endl;
    return target; // Placeholder
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] && track.get()) {
        int diff = std::abs(decks[active_deck]->get_bpm() - track->get_bpm());
        if (diff <= bpm_tolerance) {
            return true;
        }
    }
    return false;
}

/**
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    if (decks[active_deck] && track.get()) {
        int original_bpm = track->get_bpm();
        track->set_bpm((original_bpm + decks[active_deck]->get_bpm()) / 2);
        std::cout << "[Sync BPM] Syncing BPM from "<< original_bpm << " to " << track->get_bpm() << std::endl;
    }
}
