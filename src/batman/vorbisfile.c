/* batman/, file name from libvorbis (vorbisfile.c, linked between pcbatman.cpp
 * and pcapi.cpp); unproven. OggVorbis_File as in libvorbis 1.x. */

#include "../nu2api/nucore/common.h"
#include <stdlib.h>
#include <string.h>

typedef __int64 ogg_int64_t;

#define OV_EINVAL -131
#define OPENED 2

typedef struct vorbis_info {
  int pad[8];
} vorbis_info;

typedef struct vorbis_comment {
  int pad[4];
} vorbis_comment;

typedef struct OggVorbis_File {
  void *datasource;         /* 0x00 */
  int seekable;             /* 0x04 */
  ogg_int64_t offset;       /* 0x08 */
  ogg_int64_t end;          /* 0x10 */
  int oy[7];                /* 0x18, ogg_sync_state */
  int links;                /* 0x34 */
  ogg_int64_t *offsets;     /* 0x38 */
  ogg_int64_t *dataoffsets; /* 0x3c */
  long *serialnos;          /* 0x40 */
  ogg_int64_t *pcmlengths;  /* 0x44 */
  vorbis_info *vi;          /* 0x48 */
  vorbis_comment *vc;       /* 0x4c */
  ogg_int64_t pcm_offset;   /* 0x50 */
  int ready_state;          /* 0x58 */
  int pad5c[(0x78 - 0x5c) / 4];
  int os[(0x1e0 - 0x78) / 4];          /* 0x78, ogg_stream_state */
  int vd[(0x250 - 0x1e0) / 4];         /* 0x1e0, vorbis_dsp_state */
  int vb[(0x2c0 - 0x250) / 4];         /* 0x250, vorbis_block */
  void *read_func;                     /* 0x2c0, ov_callbacks */
  void *seek_func;                     /* 0x2c4 */
  int (*close_func)(void *datasource); /* 0x2c8 */
  void *tell_func;                     /* 0x2cc */
} OggVorbis_File;

int vorbis_block_clear(void *vb);
void vorbis_dsp_clear(void *vd);
int ogg_stream_clear(void *os);
void vorbis_info_clear(vorbis_info *vi);
void vorbis_comment_clear(vorbis_comment *vc);
int ogg_sync_clear(void *oy);

// FUNCTION: LEGOBATMAN 0x0053c4b0
int ov_clear(OggVorbis_File *vf) {
  if (vf) {
    vorbis_block_clear(vf->vb);
    vorbis_dsp_clear(vf->vd);
    ogg_stream_clear(vf->os);
    if (vf->vi && vf->links) {
      int i;
      for (i = 0; i < vf->links; i++) {
        vorbis_info_clear(vf->vi + i);
        vorbis_comment_clear(vf->vc + i);
      }
      free(vf->vi);
      free(vf->vc);
    }
    if (vf->dataoffsets)
      free(vf->dataoffsets);
    if (vf->pcmlengths)
      free(vf->pcmlengths);
    if (vf->serialnos)
      free(vf->serialnos);
    if (vf->offsets)
      free(vf->offsets);
    ogg_sync_clear(vf->oy);
    if (vf->datasource && vf->close_func)
      (vf->close_func)(vf->datasource);
    memset(vf, 0, sizeof(*vf));
  }
  return 0;
}

// FUNCTION: LEGOBATMAN 0x0053c7a0
ogg_int64_t ov_pcm_total(OggVorbis_File *vf, int i) {
  if (vf->ready_state < OPENED)
    return OV_EINVAL;
  if (!vf->seekable || i >= vf->links)
    return OV_EINVAL;
  if (i < 0) {
    ogg_int64_t acc = 0;
    int i;
    for (i = 0; i < vf->links; i++)
      acc += ov_pcm_total(vf, i);
    return acc;
  } else {
    return vf->pcmlengths[i * 2 + 1];
  }
}

// FUNCTION: LEGOBATMAN 0x0053d850
ogg_int64_t ov_pcm_tell(OggVorbis_File *vf) {
  if (vf->ready_state < OPENED)
    return OV_EINVAL;
  return vf->pcm_offset;
}
