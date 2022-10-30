#ifndef IJVM_STATE_H
#define IJVM_STATE_H

#include <stack.h>
#include <ijvm.h>

typedef struct frame
{
  struct frame* previous;
  word_t* vars;
  int pc;
  uint16_t arg_count;
  uint16_t var_count;
} frame_t;

typedef struct
{
  word_t origin;
  word_t size;
  byte_t *data;
} block;

typedef struct
{
  FILE *in;
  FILE *out;
  bool halted;
  bool wide;
  frame_t *current_frame;
  stack stack;
  block constants;
  block text;
} ijvm_state;


word_t load_constant(byte_t* num)
{
  return((num[0] & 0xFF) << 24) | ((num[1] & 0xFF) <<16) | ((num[2] & 0xFF) << 8) | (num[3] & 0xFF);
}

int16_t get_short(ijvm_state* current_state, int pc)
{ 
  int16_t temp = current_state->text.data[pc];
  temp <<= 8;
  temp |= current_state->text.data[pc + 1];
  return temp;
}

uint16_t get_ushort(ijvm_state* current_state, int pc)
{ 
  uint16_t temp = current_state->text.data[pc];
  temp <<= 8;
  temp |= current_state->text.data[pc + 1];
  return temp;
}

static uint32_t swap_uint32(uint32_t num)
{
  return((num>>24)&0xff) | ((num<<8)&0xff0000) |((num>>8)&0xff00) | ((num<<24)&0xff000000);
}

void init_frame(frame_t *current_frame, uint16_t frame_size)
{
  current_frame->vars = malloc(sizeof current_frame->vars * frame_size);
	current_frame->previous = 0;
	current_frame->pc = 0;
	current_frame->arg_count = 0;
}

void create_frame(ijvm_state *current_state, uint16_t frame_size)
{
  frame_t *new_frame = malloc(sizeof new_frame);
  init_frame(new_frame, frame_size);
  new_frame->previous = current_state->current_frame;
  current_state->current_frame = new_frame;
}

void to_previous(ijvm_state *current_state)
{
  frame_t* temp = current_state->current_frame->previous;
  free(current_state->current_frame->vars);
  free(current_state->current_frame);
  current_state->current_frame = temp;
  current_state->current_frame->pc++;
}

#endif