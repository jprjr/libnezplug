#ifndef AUDIOSYS_H__
#define AUDIOSYS_H__

#include <nezplug/nezplug.h>

#ifdef __cplusplus
extern "C" {
#endif

enum
{
   NES_AUDIO_FILTER_NONE,
   NES_AUDIO_FILTER_LOWPASS,
   NES_AUDIO_FILTER_WEIGHTED,
   NES_AUDIO_FILTER_LOWPASS_WEIGHTED
};


void NESAudioRender(NEZ_PLAY*, int16_t *bufp, uint32_t buflen);
void NESAudioHandlerInstall(NEZ_PLAY *, const NEZ_NES_AUDIO_HANDLER * ph);
void NESAudioHandlerTerminate(NEZ_PLAY *);
void NESAudioFrequencySet(NEZ_PLAY *, uint32_t freq);
uint32_t NESAudioFrequencyGet(NEZ_PLAY *);
void NESAudioChannelSet(NEZ_PLAY *, uint32_t ch);
uint32_t NESAudioChannelGet(NEZ_PLAY *);
void NESAudioHandlerInitialize(NEZ_PLAY *);
void NESVolumeHandlerInstall(NEZ_PLAY *, const NEZ_NES_VOLUME_HANDLER * ph);
void NESVolumeHandlerTerminate(NEZ_PLAY *);
void NESVolume(NEZ_PLAY *, uint32_t volume);
void NESAudioFilterSet(NEZ_PLAY*, uint32_t filter);

#ifdef __cplusplus
}
#endif

#endif /* AUDIOSYS_H__ */
