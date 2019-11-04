#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "../src/include/nezplug/nezplug.h"

static uint8_t *slurp(char *filename, uint32_t *size);

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
    /* FILE *out; */
    uint8_t *data;
    uint32_t size;
    uint8_t *m3uData;
    uint32_t m3uSize;
    /* unsigned int i; */
    char *p;
    char *filename;
    int16_t *buffer;
    /* uint32_t samples; */

    if(argc < 2) {
        fprintf(stderr,"Usage: decode-all /path/to/file\n");
        fprintf(stderr,"  decodes all tracks to PCM\n");
        return 1;
    }

    filename = NULL;
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
        /*
        fprintf(stderr,"Total tracks: %u\n",player->tracks->total);
        if(player->tracks->title) fprintf(stderr,"title: %s\n",player->tracks->title);
        if(player->tracks->artist) fprintf(stderr,"artist: %s\n",player->tracks->artist);
        if(player->tracks->copyright) fprintf(stderr,"copyright: %s\n",player->tracks->copyright);
        if(player->tracks->dumper) fprintf(stderr,"dumper: %s\n",player->tracks->dumper);
        */
    }

    NEZSetFrequency(player,48000);
    NEZSetChannel(player,2);
    buffer = (int16_t *)malloc(sizeof(int16_t) * NEZGetChannel(player) * 4096);
    if(buffer == NULL) {
        NEZDelete(player);
        free(data);
        return 1;
    }

#if 0
    filename = NULL;
    for(i=NEZGetSongStart(player);i<=NEZGetSongMax(player);i++) {
        size = snprintf(NULL,0,"%s.track_%02d.pcm",argv[1],i) + 1;
        filename = realloc(filename,size);
        if(filename == NULL) {
            NEZDelete(player);
            free(buffer);
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
            free(buffer);
            free(data);
            return 1;
        }
        NEZSetSongNo(player,i);
        NEZReset(player);

        /* convert 2 minutes of audio */
        samples = 0;
        while(samples < 48000 * 120) {
            NEZRender(player,buffer,4096);
            if(fwrite(buffer,1,4096 * sizeof(int16_t) * 2,out) !=
                4096 * sizeof(int16_t) * 2) {
                fprintf(stderr,"Error writing data\n");
                NEZDelete(player);
                free(buffer);
                free(data);
            }
            samples += 4096;
        }

        fclose(out);

    }
#endif


    NEZDelete(player);
    free(filename);
    free(buffer);
    return 0;
}





