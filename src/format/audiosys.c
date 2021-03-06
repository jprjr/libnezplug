#include "audiosys.h"
#include "../normalize.h"
#include <stdio.h>

/* ---------------------- */
/*  Audio Render Handler  */
/* ---------------------- */

#define SHIFT_BITS 8

PROTECTED void NESAudioFilterSet(NEZ_PLAY *pNezPlay, uint32_t filter)
{
	pNezPlay->naf_type = filter;
	pNezPlay->naf_prev[0] = 0x8000;
	pNezPlay->naf_prev[1] = 0x8000;
	pNezPlay->output2[0] = 0x7fffffff;
	pNezPlay->output2[1] = 0x7fffffff;
}

PROTECTED void NESAudioRender(NEZ_PLAY *pNezPlay, int16_t *bufp, uint32_t buflen)
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
				int32_t clamp;
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
					if (pNezPlay->output2[ch] == 0x7fffffff){
						pNezPlay->output2[ch] = ((int32_t)output[ch]<<14) - 0x40000000;
					}
					pNezPlay->output2[ch] -= (pNezPlay->output2[ch] - (((int32_t)output[ch]<<14) - 0x40000000))/(64*pNezPlay->filter);
					buffer =  output[ch]/2 - pNezPlay->output2[ch]/0x8000;

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
							output[ch] = (output[ch] * pNezPlay->lowlevel + prev * pNezPlay->filter) / (pNezPlay->lowlevel+pNezPlay->filter);
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
							output[ch] = (output[ch] * pNezPlay->lowlevel + prev * pNezPlay->filter) / (pNezPlay->lowlevel+pNezPlay->filter);
							pNezPlay->naf_prev[ch] = output[ch];
						}
						{
							uint32_t prev = pNezPlay->naf_prev[ch];
							pNezPlay->naf_prev[ch] = output[ch];
							output[ch] = (output[ch] + output[ch] + output[ch] + prev) >> 2;
						}
						break;
				}
				clamp = ((int32_t)output[ch]) - 0x8000;
				clamp *= pNezPlay->gain;
                clamp >>= 8;
				if(clamp < -32767) clamp = -32767;
				else if(clamp > 32767) clamp = 32767;
				*bufp   = (int16_t)clamp;
				bufp++;
			}
		}
	}
}

PROTECTED void NESVolume(NEZ_PLAY *pNezPlay, uint32_t volume)
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

PROTECTED void NESAudioHandlerInstall(NEZ_PLAY *pNezPlay, const NEZ_NES_AUDIO_HANDLER * ph)
{
	for (;(ph->fMode&2)?(!!ph->Proc2):(!!ph->Proc);ph++) {
		NESAudioHandlerInstallOne(pNezPlay, ph);
	}
}

PROTECTED void NESAudioHandlerTerminate(NEZ_PLAY *pNezPlay)
{
	NEZ_NES_AUDIO_HANDLER *p = pNezPlay->nah, *next;
	while (p) {
		next = p->next;
		XFREE(p);
		p = next;
	}
}

PROTECTED void NESVolumeHandlerInstall(NEZ_PLAY *pNezPlay, const NEZ_NES_VOLUME_HANDLER * ph)
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

PROTECTED void NESVolumeHandlerTerminate(NEZ_PLAY *pNezPlay)
{
	NEZ_NES_VOLUME_HANDLER *p = pNezPlay->nvh, *next;
	while (p) {
		next = p->next;
		XFREE(p);
		p = next;
	}
}

PROTECTED void NESAudioHandlerInitialize(NEZ_PLAY *pNezPlay)
{
	pNezPlay->nah = 0;
	pNezPlay->nvh = 0;
}

PROTECTED void NESAudioFrequencySet(NEZ_PLAY *pNezPlay, uint32_t freq)
{
	pNezPlay->frequency = freq;
	pNezPlay->filter = freq/3000;
	if(!pNezPlay->filter)pNezPlay->filter=1;

	pNezPlay->lowlevel = 33 - pNezPlay->lowpass_filter_level;
}

PROTECTED uint32_t NESAudioFrequencyGet(NEZ_PLAY *pNezPlay)
{
	return pNezPlay->frequency;
}

PROTECTED void NESAudioChannelSet(NEZ_PLAY *pNezPlay, uint32_t ch)
{
	pNezPlay->channel = ch;
}

PROTECTED uint32_t NESAudioChannelGet(NEZ_PLAY *pNezPlay)
{
	return pNezPlay->channel;
}

#undef SHIFT_BITS
