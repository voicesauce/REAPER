#pragma once

// Minimum mid-level C-interface for REAPER

// TODO: consistent naming rule

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../core/track.h"
#include "../epoch_tracker/epoch_tracker.h"

#ifdef __cplusplus
extern "C" {
#endif

EpochTracker* CreateEpochTracker();
void DestroyEpochTracker(EpochTracker* et);

Track* CreateTrack();
void DestroyTrack(Track* t);
int GetTrackNumFrames(Track* track);

bool InitEpochTracker(EpochTracker* et, int16_t* wave_datap, int32_t n_samples,
                      float sample_rate, float min_f0, float max_f0,
                      bool do_high_pass, bool do_hilbert_transform);
bool ComputeFeatures(EpochTracker* et);
bool TrackEpochs(EpochTracker* et);

bool GetEpochTrack(EpochTracker* et, float inter_pulse,
                   Track* pm_track);
bool GetF0AndCorrTrack(EpochTracker* et, float external_frame_interval,
                       Track* f0_track, Track* corr_track);

void GetTrackVoicedFlags(Track* track, int* voiced_flags);
void GetTrackTimes(Track* track, float* times);
void GetTrackValues(Track* track, float* values);

#ifdef __cplusplus
}
#endif
