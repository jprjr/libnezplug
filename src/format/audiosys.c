#include "audiosys.h"
#include "../normalize.h"
#include <stdio.h>

/* ---------------------- */
/*  Audio Render Handler  */
/* ---------------------- */

#define SHIFT_BITS 8

int32_t output2[2],filter;
static int lowlevel;

void NESAudioFilterSet(NEZ_PLAY *pNezPlay, uint32_t filter)
{
	pNezPlay->naf_type = filter;
	pNezPlay->naf_prev[0] = 0x8000;
	pNezPlay->naf_prev[1] = 0x8000;
	output2[0] = 0x7fffffff;
	output2[1] = 0x7fffffff;
}

void NESAudioRender(NEZ_PLAY *pNezPlay, int16_t *bufp, uint32_t buflen)
{
	uint32_t maxch = NESAudioChannelGet(pNezPlay);
	while (buflen--)
	{
		NEZ_NES_AUDIO_HANDLER *ph;
		int32_t accum[2] = { 0, 0 };
		uint32_t ch;

		for (ph = pNezPlay->nah; ph; ph = ph->next)
		{
			if (!(ph->fMode & 1) || bufp)
			{
				if (pNezPlay->channel == 2 && ph->fMode & 2)
				{
					ph->Proc2(pNezPlay, accum);
				}
				else
				{
					int32_t devout = ph->Proc(pNezPlay);
					accum[0] += devout;
					accum[1] += devout;
				}
			}
		}
		if (bufp)
		{
			for (ch = 0; ch < maxch; ch++)
			{
				uint32_t output[2];
				accum[ch] += (0x10000 << SHIFT_BITS);
				if (accum[ch] < 0)
					output[ch] = 0;
				else if (accum[ch] > (0x20000 << SHIFT_BITS) - 1)
					output[ch] = (0x20000 << SHIFT_BITS) - 1;
				else
					output[ch] = accum[ch];
				output[ch] >>= SHIFT_BITS;

				//DCオフセットフィルタ
				if(!(pNezPlay->naf_type&4)){
					int32_t buffer;
					if (output2[ch] == 0x7fffffff){
						output2[ch] = ((int32_t)output[ch]<<14) - 0x40000000;
						//output2[ch] *= -1;
					}
					output2[ch] -= (output2[ch] - (((int32_t)output[ch]<<14) - 0x40000000))/(64*filter);
					buffer =  output[ch]/2 - output2[ch]/0x8000;

					if (buffer < 0)
						output[ch] = 0;
					else if (buffer > 0xffff)
						output[ch] = 0xffff;
					else
						output[ch] = buffer;
				}else{
					output[ch] >>= 1;
				}
				switch (pNezPlay->naf_type&3)
				{
					case NES_AUDIO_FILTER_LOWPASS:
						{
							uint32_t prev = pNezPlay->naf_prev[ch];
							//output[ch] = (output[ch] + prev) >> 1;
							output[ch] = (output[ch] * lowlevel + prev * filter) / (lowlevel+filter);
							pNezPlay->naf_prev[ch] = output[ch];
						}
						break;
					case NES_AUDIO_FILTER_WEIGHTED:
						{
							uint32_t prev = pNezPlay->naf_prev[ch];
							pNezPlay->naf_prev[ch] = output[ch];
							output[ch] = (output[ch] + output[ch] + output[ch] + prev) >> 2;
						}
						break;
					case NES_AUDIO_FILTER_LOWPASS_WEIGHTED:
						{
							uint32_t prev = pNezPlay->naf_prev[ch];
							//output[ch] = (output[ch] + prev) >> 1;
							output[ch] = (output[ch] * lowlevel + prev * filter) / (lowlevel+filter);
							pNezPlay->naf_prev[ch] = output[ch];
						}
						{
							uint32_t prev = pNezPlay->naf_prev[ch];
							pNezPlay->naf_prev[ch] = output[ch];
							output[ch] = (output[ch] + output[ch] + output[ch] + prev) >> 2;
						}
						break;
				}
				*bufp++ = (int16_t)(((int32_t)output[ch]) - 0x8000);
			}
		}
	}
}

void NESVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
{
	NEZ_NES_VOLUME_HANDLER *ph;
	for (ph = pNezPlay->nvh; ph; ph = ph->next) ph->Proc(pNezPlay, volume);
}

static void NESAudioHandlerInstallOne(NEZ_PLAY *pNezPlay, const NEZ_NES_AUDIO_HANDLER *ph)
{
	NEZ_NES_AUDIO_HANDLER *nh;

	/* Add to tail of list*/
	nh = XMALLOC(sizeof(NEZ_NES_AUDIO_HANDLER));
	nh->fMode = ph->fMode;
	nh->Proc = ph->Proc;
	nh->Proc2 = ph->Proc2;
	nh->next = 0;

	if (pNezPlay->nah)
	{
		NEZ_NES_AUDIO_HANDLER *p = pNezPlay->nah;
		while (p->next) p = p->next;
		p->next = nh;
	}
	else
	{
		pNezPlay->nah = nh;
	}
}
void NESAudioHandlerInstall(NEZ_PLAY *pNezPlay, const NEZ_NES_AUDIO_HANDLER * ph)
{
	for (;(ph->fMode&2)?(!!ph->Proc2):(!!ph->Proc);ph++) {
		NESAudioHandlerInstallOne(pNezPlay, ph);
	}
}

void NESAudioHandlerTerminate(NEZ_PLAY *pNezPlay)
{
	NEZ_NES_AUDIO_HANDLER *p = pNezPlay->nah, *next;
	while (p) {
		next = p->next;
		XFREE(p);
		p = next;
	}
}

void NESVolumeHandlerInstall(NEZ_PLAY *pNezPlay, const NEZ_NES_VOLUME_HANDLER * ph)
{
	NEZ_NES_VOLUME_HANDLER *nh;

	/* Add to tail of list*/
	for (;ph->Proc;ph++)
	{
		nh = XMALLOC(sizeof(NEZ_NES_VOLUME_HANDLER));
		nh->Proc = ph->Proc;
		nh->next = pNezPlay->nvh;

		pNezPlay->nvh = nh;
	}
}

void NESVolumeHandlerTerminate(NEZ_PLAY *pNezPlay)
{
	NEZ_NES_VOLUME_HANDLER *p = pNezPlay->nvh, *next;
	while (p) {
		next = p->next;
		XFREE(p);
		p = next;
	}
}

void NESAudioHandlerInitialize(NEZ_PLAY *pNezPlay)
{
	pNezPlay->nah = 0;
	pNezPlay->nvh = 0;
}

void NESAudioFrequencySet(NEZ_PLAY *pNezPlay, uint32_t freq)
{
	pNezPlay->frequency = freq;
	filter = freq/3000;
	if(!filter)filter=1;

	lowlevel = 33 - pNezPlay->lowpass_filter_level;
}
uint32_t NESAudioFrequencyGet(NEZ_PLAY *pNezPlay)
{
	return pNezPlay->frequency;
}

void NESAudioChannelSet(NEZ_PLAY *pNezPlay, uint32_t ch)
{
	pNezPlay->channel = ch;
}

uint32_t NESAudioChannelGet(NEZ_PLAY *pNezPlay)
{
	return pNezPlay->channel;
}
