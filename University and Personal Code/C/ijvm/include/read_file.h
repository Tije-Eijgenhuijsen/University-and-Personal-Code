#include <ijvm.h>
#include <ijvm_state.h>
#include <stack.h>

bool check_file_validity(FILE *fp)
{
  //Check if file opened correctly
  if(fp == NULL)
  {
    fprintf(stderr, "Failed to open file.\n");
    return 0;
  }

  //Check file type
  word_t header;
  fread(&header, sizeof(header), 1, fp);
  header = swap_uint32(header);

  if(header != MAGIC_NUMBER)
  {
    fprintf(stderr, "Wrong type of file.\n");
    return 0;
  }
  return 1;
}

void read_block(block *blockPtr, FILE *fp)
{
  //Read origin
  fread(&blockPtr->origin, sizeof(word_t), 1, fp);
  blockPtr->origin = swap_uint32(blockPtr->origin);

  //Read size
  fread(&blockPtr->size, sizeof(word_t), 1, fp);
  blockPtr->size = swap_uint32(blockPtr->size);

  //Read data
  blockPtr->data = malloc(blockPtr->size);
  fread(blockPtr->data, sizeof(byte_t), blockPtr->size, fp);
}

int read_file(ijvm_state *current_state, char *path)
{
  //Open file
  FILE *fp;
  fp = fopen(path, "rb");

  if(!check_file_validity(fp)){ return -1; }

  read_block(&current_state->constants, fp);
  read_block(&current_state->text, fp);

  return 0;
}