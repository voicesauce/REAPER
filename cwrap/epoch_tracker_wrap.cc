// LICENSE: MIT
// Ryuichi YAMAMOTO (zryuichi@gmail.com)

#include "epoch_tracker_wrap.h"

EpochTracker* CreateEpochTracker() { return new EpochTracker(); }

void DestroyEpochTracker(EpochTracker* et) {
  if (et != NULL) {
    delete et;
  }
}

Track* CreateTrack() { return new Track; }

void DestroyTrack(Track* t) {
  if (t != NULL) {
    delete t;
  }
}

int GetTrackNumFrames(Track* track) { return track->num_frames(); }

bool InitEpochTracker(EpochTracker* et, int16_t* wave_datap, int32_t n_samples,
                      float sample_rate, float min_f0, float max_f0,
                      bool do_high_pass, bool do_hilbert_transform) {
  return et->Init(wave_datap, n_samples, sample_rate, min_f0, max_f0,
                  do_high_pass, do_hilbert_transform);
}

bool ComputeFeatures(EpochTracker* et) { return et->ComputeFeatures(); }

bool TrackEpochs(EpochTracker* et) { return et->TrackEpochs(); }

bool GetEpochTrack(EpochTracker* et, float inter_pulse,
                   Track* pm_track) {
  // TODO: how do I know the size of vectors before GetFilledEpochs?
  std::vector<float> times;
  std::vector<int16_t> voicing;
  et->GetFilledEpochs(inter_pulse, &times, &voicing);

  pm_track->resize(times.size());
  for (int32_t i = 0; i < times.size(); ++i) {
    pm_track->t(i) = times[i];
    pm_track->set_v(i, voicing[i]);
  }

  return true;
}

void GetTrackVoicedFlags(Track* track, int* voiced_flags) {
  for (int32_t i = 0; i < track->num_frames(); ++i) {
    voiced_flags[i] = track->v(i) ? 1 : 0;
  }
}

void GetTrackTimes(Track* track, float* times) {
  for (int32_t i = 0; i < track->num_frames(); ++i) {
    times[i] = track->t(i);
  }
}

void GetTrackValues(Track* track, float* values) {
  for (int32_t i = 0; i < track->num_frames(); ++i) {
    values[i] = track->a(i);
  }
}

bool GetF0AndCorrTrack(EpochTracker* et, float external_frame_interval,
                       Track* f0_track, Track* corr_track) {
  // TODO: how do I know the size of vectors before ResampleAnd.. ?
  std::vector<float> f0;
  std::vector<float> corr;
  if (!et->ResampleAndReturnResults(external_frame_interval, &f0, &corr)) {
    return false;
  }

  f0_track->resize(f0.size());
  corr_track->resize(corr.size());
  for (int32_t i = 0; i < f0.size(); ++i) {
    float t = external_frame_interval * i;
    f0_track->t(i) = t;
    corr_track->t(i) = t;
    f0_track->set_v(i, (f0[i] > 0.0) ? true : false);
    corr_track->set_v(i, (f0[i] > 0.0) ? true : false);
    f0_track->a(i) = (f0[i] > 0.0) ? f0[i] : -1.0;
    corr_track->a(i) = corr[i];
  }

  return true;
}
