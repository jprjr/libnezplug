#include <string.h>
#include "trackinfo.h"
#include "../normalize.h"

#include <stdio.h>

PROTECTED NEZ_TRACKS *TRACKS_New(void) {
    NEZ_TRACKS *tracks = (NEZ_TRACKS *)XMALLOC(sizeof(NEZ_TRACKS));
    if(tracks != NULL) {
        tracks->total = 0;
        tracks->info = NULL;
        tracks->title = NULL;
        tracks->artist = NULL;
        tracks->copyright = NULL;
        tracks->dumper = NULL;
    }
    return tracks;
}

PROTECTED void TRACKS_Delete(NEZ_TRACKS *tracks) {
    uint32_t i = 0;
    if(tracks) {
        if(tracks->info) {
            for(i=0;i<tracks->total;i++) {
                if(tracks->info[i].title != NULL) {
                    XFREE(tracks->info[i].title);
                }
            }
            XFREE(tracks->info);
        }
        if(tracks->title) {
            XFREE(tracks->title);
        }
        if(tracks->artist) {
            XFREE(tracks->artist);
        }
        if(tracks->copyright) {
            XFREE(tracks->copyright);
        }
        if(tracks->dumper) {
            XFREE(tracks->dumper);
        }
        XFREE(tracks);
    }
}

static const char *
skip_white(const char *data, uint32_t length) {
    const char *p = data;
    while(p - data < length) {
        switch(*p) {
            case ' ': /* fallthrough */
            case '\t': p++; break;/* fallthrough */
            default: return p;
        }
    }
    return NULL;
}


static void parse_comment(NEZ_TRACKS *tracks, const char *data, uint32_t length) {
    const char *p = data;
    uint32_t offset = 0;
    uint8_t tag = 0;
    (void)tracks;
    (void)length;
    char *buf = NULL;

    if( p + 1 - data >= length ) return;
    p = skip_white(++p,length - 1);
    if(p == NULL) return;

#define TAG_TEST(s,i) \
    if(length - (p - data) > (long)(sizeof(s) - 1)) { \
        if(strncasecmp(p,s,sizeof(s)-1) == 0) { \
            offset = sizeof(s) - 1; \
            tag = i; \
            goto save_tag; \
        } \
    }

    TAG_TEST("@title",1)
    TAG_TEST("title:",1)

    TAG_TEST("@artist",2)
    TAG_TEST("artist:",2)

    TAG_TEST("@composer",3)
    TAG_TEST("composer:",3)

    TAG_TEST("@date",4)
    TAG_TEST("date:",4)

    TAG_TEST("@copyright",4)
    TAG_TEST("copyright:",4)

    TAG_TEST("@ripper",5)
    TAG_TEST("ripper:",5)

    TAG_TEST("@ripping",5)
    TAG_TEST("ripping:",5)

#undef TAG_TEST

    /* other format: first line is title, other lines are tagged */
    if(tracks->title == NULL) {
        tag = 1;
        goto save_tag;
    }


save_tag:
    if(tag > 0) { /* we found something maybe */
        p += offset;
        p = skip_white(p, length - (p - data));
        if(p != NULL) {
            buf = XMALLOC(length - (p - data) + 1);
            if(buf == NULL) return;
            XMEMSET(buf,0,length - (p - data) + 1);
            XMEMCPY(buf,p,length - (p - data));
            switch(tag) {
                case 1: {
                    if(tracks->title != NULL) XFREE(tracks->title);
                    tracks->title = buf;
                }
                break;
                case 2: {
                    if(tracks->artist != NULL) break; /* @artist is lower-precedent to @composer */
                }
                break;
                case 3: {
                    if(tracks->artist != NULL) XFREE(tracks->artist);
                    tracks->artist = buf;
                }
                break;
                case 4: {
                    if(tracks->copyright != NULL) XFREE(tracks->copyright);
                    tracks->copyright = buf;
                }
                break;
                case 5: {
                    if(tracks->dumper != NULL) XFREE(tracks->dumper);
                    tracks->dumper = buf;
                }
                break;
            }
        }
    }

    return;

}

static int32_t parse_timestamp(const char *buf) {
    int32_t r = 0;
    int32_t t = 0;
    int32_t c = 0;
    const char *b = buf;

    if(strlen(b) == 0) return -1;

    while(*b) {
        if(*b >= 48 && *b <= 57) {
            t = *b - 48;
            c *= 10;
            c += t;
        }
        else if(*b == ':') {
            c *= 60;
            r += c;
            c = 0;
        }
        else {
            break;
        }
        b++;
    }
    r += c;
    return r * 1000;
}

static int32_t parse_tracknum(const char *buf) {
    /* returns -1 on error */
    int32_t r = 0;
    int32_t t = 0;
    const char *b = buf;

    if(strlen(b) == 0) return -1;

    if(*b == '$') {
        b++;
        while(*b) {
          if(*b >= 48 && *b <= 57) {
              t = *b - 48;
          } else if(*b >= 65 && *b <= 70) {
              t = *b - 55;
          } else if(*b >= 97 && *b <= 102) {
              t = *b - 87;
          } else {
              return -1;
          }
          r *= 16;
          r += t;
          b++;
        }
        return r;
    }

    while(*b) {
        if(*b >= 48 && *b <= 57) {
            t = *b - 48;
        } else {
            return -1;
        }
        r *= 10;
        r += t;
        b++;
    }
    return r;
}

static const char *find_comma(const char *data, uint32_t length) {
    const char *p = data;
    while(p - data < length) {
        if(*p == '\\') {
            p+=2;
            continue;
        }
        if(*p == ',') return p;
        p++;
    }
    return p;
}

static void tracks_process_line(NEZ_TRACKS *tracks, const char *data, uint32_t length) {
    uint32_t i = 0;
    int32_t temp = 0;
    const char *p = data;
    const char *d = data;
    char buf[101];
    (void)tracks;

    if(*p == '#') {
        parse_comment(tracks,d,length);
        return;
    }

    if(length == 0) {
        return;
    }

    /* filename::gbs */
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i] = 0;

    if(p - data == length) {
        return;
    }
    d = p+1;

    /* track number */
    i = 0;
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i] = 0;

    temp = parse_tracknum(buf);
    if(temp == -1) return;

    /* this is the bare minimum needed to add a trackinfo */
    tracks->total++;
    tracks->info = XREALLOC(tracks->info, sizeof(NEZ_TRACK_INFO) * tracks->total);
    if(tracks->info == NULL) return;

    tracks->info[tracks->total - 1].songno = (uint32_t)temp;
    tracks->info[tracks->total - 1].title = NULL;
    tracks->info[tracks->total - 1].fade_ms = -1;
    tracks->info[tracks->total - 1].length_ms = -1;
    tracks->info[tracks->total - 1].intro_ms = -1;
    tracks->info[tracks->total - 1].loop_ms = -1;
    tracks->info[tracks->total - 1].loops = -1;

    if(p - data == length) {
        return;
    }
    d = p+1;

    /* song title */
    i = 0;
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i++] = 0;

    fprintf(stderr,"song title, i = %d\n",i);

    tracks->info[tracks->total - 1].title = XMALLOC(i);
    if(tracks->info[tracks->total - 1].title == NULL) return;
    XMEMCPY(tracks->info[tracks->total - 1].title,buf,i);
    if(p - data == length) {
        return;
    }
    d = p+1;

    /* track length */
    i = 0;
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i] = 0;
    temp = parse_timestamp(buf);
    tracks->info[tracks->total - 1].length_ms = temp;

    if(p - data == length) {
        return;
    }
    d = p+1;

    /* loop time */
    i = 0;
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i] = 0;

    if(buf[0] == '-') {
        tracks->info[tracks->total - 1].loop_ms =
          tracks->info[tracks->total - 1].length_ms;
    }
    else {
        temp = parse_timestamp(buf);
        if(temp != -1) {
            if(buf[i-1] == '-') {
                tracks->info[tracks->total - 1].intro_ms = temp;
                tracks->info[tracks->total - 1].loop_ms = tracks->info[tracks->total - 1].length_ms - tracks->info[tracks->total - 1].length_ms;
            }
            else {
                tracks->info[tracks->total - 1].loop_ms = temp;
                tracks->info[tracks->total - 1].intro_ms = tracks->info[tracks->total - 1].length_ms - tracks->info[tracks->total - 1].loop_ms;

            }
        }
    }

    if(p - data == length) {
        return;
    }
    d = p+1;

    /* track fade */
    i = 0;
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i] = 0;
    temp = parse_timestamp(buf);
    tracks->info[tracks->total - 1].fade_ms = temp;
    if(p - data == length) {
        return;
    }
    d = p+1;

    /* track loops */
    i = 0;
    XMEMSET(buf,0,101);
    p = find_comma(d,length - (d - data));
    while(d < p) {
        if(*d == '\\') {
            d++;
        }
        buf[i++] = *d++;
        if(i > 99) {
            d = p; break;
        }
    }
    buf[i] = 0;
    if(p - data == length) {
        return;
    }
    d = p+1;

}

PROTECTED uint8_t TRACKS_LoadM3U(NEZ_TRACKS *tracks, const uint8_t *uData, uint32_t length) {
    (void)tracks;
    (void)length;
    uint32_t line = 1;
    const char *data = (const char *)uData;
    const char *d = data;
    const char *p = d;
    uint8_t offset = 0;
    uint32_t i = 0;

    do {
        p = memchr(d,'\n',length - (d - data));
        if(p == NULL) {
            p = memchr(d,'\r',length - (d - data));
            if(p == NULL) p = data + length;
        }
        offset = 0;

        if(*p == '\n') {
            if (p - 1 >= d && *(p-1) == '\r') {
                offset = 1;
            }
        }

        tracks_process_line(tracks,d,p - d - offset);
        p++;
        d = p;
        line++;
    } while(p - data < length);

    printf("Track count: %d\n",tracks->total);
    for(i=0;i<tracks->total;i++) {
        printf("Track %02d: \n",i);
        printf("\ttitle: %s\n", tracks->info[i].title);
        printf("\tlength: %d\n", tracks->info[i].length_ms);
        printf("\tintro_length: %d\n", tracks->info[i].intro_ms);
        printf("\tloop_length: %d\n", tracks->info[i].loop_ms);
        printf("\tplay_length: %d\n", tracks->info[i].length_ms);
        printf("\tfade_length: %d\n", tracks->info[i].fade_ms);
    }

    return 1;
}
