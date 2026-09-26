#ifndef STANDARD_REVERB_ZONE_PROPERTIES_H
#define STANDARD_REVERB_ZONE_PROPERTIES_H

//density limits
const float default_density = 1.0f;
const float min_density = 0.0f;
const float max_density = 1.0f;

// diffusion 
const float default_diffusion = 1.0f;
const float min_diffusion = 0.0f; 
const float max_diffusion = 1.0f;

//gain value
const float default_gain = 0.32f;
const float min_gain = 0.0f;
const float max_gain = 1.0f;

//gain value high frequency
const float default_gain_hf = 0.89f;
const float min_gain_hf = 0.0f;
const float max_gain_hf = 0.99f;


//decay value
const float default_decay = 1.49f;
const float min_decay = 0.1f;
const float max_decay = 20.0f;

//decay value high frequency
const float default_decay_hf = 0.83f;
const float min_decay_hf = 0.1f;
const float max_decay_hf = 2.0f;

//reflections gain
const float default_reflections_gain = 0.05f;
const float min_reflections_gain = 0.0f;
const float max_reflections_gain = 3.16f;

//reflections delay
const float default_reflections_delay = 0.007f;
const float min_reflections_delay = 0.0f;
const float max_reflections_delay = 0.3f;

//late reverb gain
const float default_late_reverb_gain = 1.26f;
const float min_late_reverb_gain = 0.0f;
const float max_late_reverb_gain = 10.0f;

//late reverb delay
const float default_late_reverb_delay = 0.011f;
const float min_late_reverb_delay = 0.0f;
const float max_late_reverb_delay = 0.1f;

//air absorption
const float default_air_absorption = 0.994f;
const float min_air_absorption = 0.892f;
const float max_air_absorption = 1.0f;

//room rolloff
const float default_room_rolloff = 0.0f;
const float min_room_rolloff = 0.0f;
const float max_room_rolloff = 10.0f;


#endif