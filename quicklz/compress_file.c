/* SPDX-License-Identifier: GPL-1.0-only OR GPL-2.0-only OR GPL-3.0-only */

/* Sample demo for QuickLZ 1.5.x */

/*
 * Copyright (c) 2006-2011 Lasse Mikkel Reinhold <lar@quicklz.com>
 * Copyright (c) 2023 Jeffrey H. Johnson <trnsz@pobox.com>
 */

/*
 * Remember to define QLZ_COMPRESSION_LEVEL and QLZ_STREAMING_MODE
 * to the same values for the compressor and decompressor.
 */

#include <stdio.h>
#include <stdlib.h>

#include "quicklz.h"

int
main(int argc, char *argv[])
{
  FILE *               ifile, *ofile;
  char *               src, *dst;
  qlz_state_compress * state_compress
    = (qlz_state_compress *)malloc(sizeof ( qlz_state_compress ));
  size_t               len, len2;

  ifile  = fopen(argv[1], "rb");
  ofile  = fopen(argv[2], "wb");

  /* Allocate source buffer and read file */
  fseek(ifile, 0, SEEK_END);
  len  = ftell(ifile);
  fseek(ifile, 0, SEEK_SET);
  src  = (char *)malloc(len);
  fread(src, 1, len, ifile);

  /* Allocate "uncompressed size" + 400 for the destination buffer */
  dst = (char *)malloc(len + 400);

  /* Compress and write result */
  len2 = qlz_compress(src, dst, len, state_compress);
  fwrite(dst, len2, 1, ofile);
  fclose(ifile);
  fclose(ofile);
  return 0;
}
