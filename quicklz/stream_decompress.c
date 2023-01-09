/* SPDX-License-Identifier: GPL-1.0-only OR GPL-2.0-only OR GPL-3.0-only */

/* Sample demo for QuickLZ 1.5.x */

/*
 * Copyright (c) 2006-2011 Lasse Mikkel Reinhold <lar@quicklz.com>
 * Copyright (c) 2023 Jeffrey H. Johnson <trnsz@pobox.com>
 */

/*
 * Remember to define QLZ_COMPRESSION_LEVEL to the same
 * value for the compressor and decompressor.
 */

#include <stdio.h>
#include <stdlib.h>

#include "quicklz.h"

#if QLZ_STREAMING_BUFFER == 0
# error Define QLZ_STREAMING_BUFFER to a non-zero value for this demo
#endif /* if QLZ_STREAMING_BUFFER == 0 */

int
main(int argc, char *argv[])
{
  FILE *                 ifile, *ofile;
  char *                 file_data, *decompressed;
  size_t                 d, c;
  qlz_state_decompress * state_decompress
    = (qlz_state_decompress *)malloc(sizeof ( qlz_state_decompress ));

  /* Read source file */
  ifile  = fopen(argv[1], "rb");
  ofile  = fopen(argv[2], "wb");

  /*
   * Acompressed packet can be at most "uncompressed size" + 400 bytes large
   * where "uncompressed size" = 10000 in worst case in this sample demo.
   */

  file_data = (char *)malloc(10000 + 400);

  /* Allocate decompression buffer */
  decompressed = (char *)malloc(10000);

  /*
   * Allocate and initially zero out the scratch buffer. After this,
   * make sure it is preserved across calls and never modified manually.
   */

  memset(state_decompress, 0, sizeof ( qlz_state_decompress ));

  /*
   * Read 9-byte header to find the size of the entire
   * compressed packet, and then read remaining packet.
   */

  while (( c = fread(file_data, 1, 9, ifile)) != 0)
    {
      c  = qlz_size_compressed(file_data);
      fread(file_data + 9, 1, c - 9, ifile);
      d  = qlz_decompress(file_data, decompressed, state_decompress);
      printf(
        "%u bytes decompressed into %u.\n",
        (unsigned int)c,
        (unsigned int)d);
      fwrite(decompressed, d, 1, ofile);
    }
  fclose(ifile);
  fclose(ofile);
  return 0;
}
