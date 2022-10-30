#include <ijvm.h>
#include <stdlib.h>
#include <stack.h>
#include <ijvm_state.h>

void jump(ijvm_state *current_state)
{
	//fprintf(stderr, "Current pc: %i\n", current_state->pc);
	current_state->current_frame->pc++;
	int16_t jump_by = get_short(current_state, current_state->current_frame->pc);
	//fprintf(stderr, "Jump_by = %X\n", jump_by);
	current_state->current_frame->pc += jump_by - 2;
	//fprintf(stderr, "New pc: %i\n", current_state->pc);	
}

void execute_instruction(byte_t instruction, ijvm_state *current_state)
{
	byte_t byte_index;
	uint16_t short_index;

	int8_t operand1, operand2;
	word_t result;

	switch (instruction)
	{
		case OP_BIPUSH:
			current_state->current_frame->pc++;
			push(&current_state->stack, (int8_t) get_instruction());
			break;
		case OP_DUP:
			push(&current_state->stack, top(&current_state->stack));
			break;
		case OP_ERR:
			fprintf(stderr, "Something went wrong.\n");
			current_state->halted = 0;
			break;
		case OP_GOTO:
			jump(current_state);
			break;
		case OP_HALT:
			current_state->halted = 0;
			break;
		case OP_IADD:
			operand2 = pop(&current_state->stack);
			operand1 = pop(&current_state->stack);
			result = operand1 + operand2;
			push(&current_state->stack, result);
			break;
		case OP_IAND:
			operand2 = pop(&current_state->stack);
			operand1 = pop(&current_state->stack);
			result = operand1 & operand2;
			push(&current_state->stack, result);
			break;
		case OP_IFEQ:
			if(pop(&current_state->stack) == 0)
			{
				jump(current_state);
			}else{
				current_state->current_frame->pc += 2;
			}
			break;
		case OP_IFLT:
			if(pop(&current_state->stack) < 0)
			{
				jump(current_state);
			}else{
				current_state->current_frame->pc += 2;
			}
			break;
		case OP_ICMPEQ:
			if(pop(&current_state->stack) == pop(&current_state->stack))
			{
				jump(current_state);
			}else{
				current_state->current_frame->pc += 2;
			}
			break;
		case OP_IINC:
			current_state->current_frame->pc++;
			byte_index = current_state->text.data[current_state->current_frame->pc];
			byte_index += current_state->current_frame->arg_count;

			current_state->current_frame->pc++;
			current_state->current_frame->vars[byte_index] += (int8_t) current_state->text.data[current_state->current_frame->pc];
			break;
		case OP_ILOAD:
			if(current_state->wide)
			{
				current_state->current_frame->pc++;
				short_index = get_ushort(current_state, current_state->current_frame->pc);
				push(&current_state->stack, current_state->current_frame->vars[short_index]);
				current_state->wide = 0;
				current_state->current_frame->pc++;
			}else{
				current_state->current_frame->pc++;
				byte_index = current_state->text.data[current_state->current_frame->pc];
				push(&current_state->stack, current_state->current_frame->vars[byte_index]);
			}
			break;
		case OP_IN: ;
			char buffer = getc(current_state->in);
			
			if(buffer == EOF)
			{
				push(&current_state->stack, 0);
			}else{
				push(&current_state->stack, (int8_t) buffer);
			}
			break;
		case OP_INVOKEVIRTUAL:
			current_state->current_frame->pc++;
			short_index = get_ushort(current_state, current_state->current_frame->pc);

			//Read new pc
			int new_pc = get_constant(short_index);

			//Read short for number of arguments and variables
			uint16_t new_argcount = get_ushort(current_state, new_pc);
			new_pc += 2;
			uint16_t new_varcount = get_ushort(current_state, new_pc);

			//Create new frame
			create_frame(current_state, new_argcount + new_varcount);

			current_state->current_frame->arg_count = new_argcount;
			current_state->current_frame->var_count = new_varcount;
			current_state->current_frame->pc = new_pc;

			//Put args into local variables
			for(int i = current_state->current_frame->arg_count - 1; i >= 0; i--)
			{
				current_state->current_frame->vars[i] = pop(&current_state->stack);
			}

			current_state->current_frame->pc++;
			break;
		case OP_IOR:
			operand2 = pop(&current_state->stack);
			operand1 = pop(&current_state->stack);
			result = operand1 | operand2;
			push(&current_state->stack, result);
			break;
		case OP_IRETURN:
			to_previous(current_state);
			break;
		case OP_ISTORE:
			if(current_state->wide)
			{
				current_state->current_frame->pc++;
				short_index = get_ushort(current_state, current_state->current_frame->pc);
				current_state->current_frame->vars[short_index] = pop(&current_state->stack);
				current_state->current_frame->pc++;
			}else{
				current_state->current_frame->pc++;
				byte_index = current_state->text.data[current_state->current_frame->pc];
				current_state->current_frame->vars[byte_index] = pop(&current_state->stack);
			}
			break;
		case OP_ISUB:
			operand2 = pop(&current_state->stack);
			operand1 = pop(&current_state->stack);
			result = operand1 - operand2;
			push(&current_state->stack, result);
			break;
		case OP_LDC_W:
			current_state->current_frame->pc++;
			short_index = get_ushort(current_state, current_state->current_frame->pc);
			push(&current_state->stack, load_constant(current_state->constants.data + (short_index * 4)));
			current_state->current_frame->pc++;
			break;
		case OP_NOP:
			break;
		case OP_OUT:;
			char temp = pop(&current_state->stack);
			fprintf(current_state->out, "%c", temp);
			break;
		case OP_POP:
			pop(&current_state->stack);
			break;
		case OP_SWAP:
			operand2 = pop(&current_state->stack);
			operand1 = pop(&current_state->stack);
			push(&current_state->stack, operand2);
			push(&current_state->stack, operand1);
			break;
		case OP_WIDE:
			current_state->wide = 1;
			break;
		default:
			fprintf(stderr, "Did not recognise instruction: %X\n", instruction);
			current_state->halted = 0;
	}
}