#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../src/include/nezplug/nezplug.h"

#if 0
static int16_t fade(int16_t m, uint32_t n, uint32_t d);
#endif

static uint8_t *slurp(char *filename, uint32_t *size);

#if 0
static int16_t fade(int16_t m, uint32_t n, uint32_t d) {
    double t = (double) n / d;
    t = 1.0f - t;
    t *= t;
    t *= (double)m;
    return (int16_t)t;
}
#endif

static uint8_t *slurp(char *filename, uint32_t *size) {
    uint8_t *buf;
    FILE *f = fopen(filename,"rb");
    if(f == NULL) {
        fprintf(stderr,"Error opening %s: %s\n",
          filename,
          strerror(errno));
        return NULL;
    }
    fseek(f,0,SEEK_END);
    *size = ftell(f);
    fseek(f,0,SEEK_SET);

    buf = (uint8_t *)malloc(*size);
    if(buf == NULL) {
        fprintf(stderr,"out of memory\n");
        return NULL;
    }
    if(fread(buf,1,*size,f) != *size) {
        fprintf(stderr,"error reading file\n");
        free(buf);
        return NULL;
    }
    return buf;
}

int main(int argc, char *argv[]) {
    NEZ_PLAY *player;
    FILE *out;
    uint8_t *data;
    uint32_t size;
    uint8_t *m3uData;
    uint32_t m3uSize;
    unsigned int i;
    char *p;
    char *filename;
    int16_t *buffer;
    uint32_t samples;
    const int channels = 2;

    if(argc < 2) {
        fprintf(stderr,"Usage: decode-all /path/to/file\n");
        fprintf(stderr,"  decodes all tracks to PCM\n");
        return 1;
    }

    filename = NULL;
    buffer = NULL;
    m3uData = NULL;
    m3uSize = 0;

    data = slurp(argv[1],&size);
    if(data == NULL) return 1;

    p = strrchr(argv[1],'.');
    if(p) {
        *p = 0;
        m3uSize = snprintf(NULL,0,"%s.m3u",argv[1]);
        filename = (char *)malloc(m3uSize + 1);
        if(filename == NULL) {
            free(data);
            return 1;
        }
        snprintf(filename,m3uSize+1,"%s.m3u",argv[1]);
        fprintf(stderr,"Trying to load %s\n",filename);
        m3uData = slurp(filename, &m3uSize);
        if(m3uData == NULL) m3uSize = 0;
        free(filename); filename = NULL;
    }

    player = NEZNew();
    if(player == NULL) {
        free(data);
        return 1;
    }

    if(NEZLoad(player,data,size)) {
        NEZDelete(player);
        free(data);
        return 1;
    }
    free(data);

    if(m3uData) {
        NEZLoadM3U(player,m3uData,m3uSize);
    }
    fprintf(stderr,"Total tracks: %u\n",player->tracks->total);
    if(NEZGetGameTitle(player)) fprintf(stderr,"title: %s\n",NEZGetGameTitle(player));
    if(NEZGetGameArtist(player)) fprintf(stderr,"artist: %s\n",NEZGetGameArtist(player));
    if(NEZGetGameCopyright(player)) fprintf(stderr,"copyright: %s\n",NEZGetGameCopyright(player));
    if(NEZGetGameDetail(player)) fprintf(stderr,"detail: %s\n",NEZGetGameDetail(player));
    if(player->tracks->dumper) fprintf(stderr,"dumper: %s\n",player->tracks->dumper);
    for(i=NEZGetSongStart(player);i<=NEZGetSongMaxAbsolute(player);i++) {
        fprintf(stderr,"Track %02d: \n"
        "\ttitle: %s\n"
        "\tlength: %u\n"
        "\tintro: %u\n"
        "\tloop: %d\n"
        "\tfade: %u\n"
        "\tloops: %u\n",
          i,
          NEZGetTrackTitle(player,i),
          NEZGetTrackLength(player,i),
          NEZGetTrackIntro(player,i),
          NEZGetTrackLoop(player,i),
          NEZGetTrackFade(player,i),
          NEZGetTrackLoops(player,i));

     }

    NEZSetFrequency(player,48000);
    NEZSetChannel(player,channels);

    filename = NULL;
    for(i=NEZGetSongStart(player);i<=NEZGetSongMax(player);i++) {
        size = snprintf(NULL,0,"%s.track_%02d.pcm",argv[1],i) + 1;
        filename = realloc(filename,size);
        if(filename == NULL) {
            NEZDelete(player);
            if(buffer != NULL) free(buffer);
            free(data);
            return 1;
        }
        snprintf(filename,size,"%s.track_%02d.pcm",argv[1],i);
        fprintf(stderr,"Decoding track #%d/%d to %s\n",i,NEZGetSongMax(player),filename);
        out = fopen(filename,"wb");
        if(out == NULL) {
            fprintf(stderr,"Error opening output file %s: %s\n",
              filename,
              strerror(errno));
            NEZDelete(player);
            if(buffer != NULL) free(buffer);
            free(data);
            return 1;
        }
        NEZSetSongNo(player,i);
        NEZReset(player);

        if(buffer == NULL) {
          fprintf(stderr,"mallocing buffer for %d samples, %d channels\n",channels * 4096, channels);
          buffer = (int16_t *)malloc(sizeof(int16_t) * channels * 4096);
          if(buffer == NULL) {
              NEZDelete(player);
              free(data);
              return 1;
          }
        }

        samples = 0;
        while(samples < 48 * (NEZGetTrackLength(player,i))) {
            NEZRender(player,buffer,4096);
            if(fwrite(buffer,1,4096 * sizeof(int16_t) * channels,out) !=
                4096 * sizeof(int16_t) * channels) {
                fprintf(stderr,"Error writing data\n");
                NEZDelete(player);
                free(buffer);
                free(data);
            }
            samples += 4096;
        }

        fclose(out);

    }


    NEZDelete(player);
    free(filename);
    free(buffer);
    return 0;
}





