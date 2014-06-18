#include <stdio.h>
#include <malloc.h>
#include "dac_read.h"

// vi: ts=2 sw=2

bool read_dac_file(const char *fn, DACData *df) {
  FILE *fp = fopen(fn, "rb");
  if (!fp) {
    perror("unable to open file");
    return false;
  }

  if (!df) {
      perror("DACData not specified");
      return false;
  }

  if (df->table) {
      free(df->table);
      df->table = NULL;
  }

  if (fread(&df->table_size, sizeof(unsigned int), 1, fp) <= 0) {
    perror("unable to read file");
    goto fail;
  }

  if (fread(&df->scale_factor, sizeof(unsigned int), 1, fp) <= 0) {
    perror("unable to read file");
    goto fail;
  }

  if (df->table_size == 0) {
    perror("empty dac table");
    goto fail;
  }

  df->table = (int *)malloc(sizeof(int) * df->table_size);
  if (!df->table) {
    perror("failed to allocate dac table memory");
    goto fail;
  }

  if (fread(df->table, sizeof(int), df->table_size, fp) != df->table_size) {
    perror("unable to read table");
    goto fail;
    return false;
  }
  
  fclose(fp);
  return true;

fail:
  fclose(fp);
  if (df->table) {
    free(df->table);
    df->table = NULL;
  }
  return false;
}


