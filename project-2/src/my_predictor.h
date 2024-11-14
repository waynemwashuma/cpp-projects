#include <string.h>
#include "branch.h"
#include "predictor.h"
#include <math.h>

#define RAS_LENGTH 30
#define N_BIT_LENGTH 22
// my_predictor.h
// This file contains a sample my_predictor class.
// It is a simple 32,768-entry gshare with a history length of 15.
// Note that this predictor doesn't use the whole 32 kilobytes available
// for the CBP-2 contest; it is just an example.

class my_update : public branch_update
{
public:
	unsigned int index;
};

class my_predictor : public branch_predictor
{
public:
	unsigned char map[1 << N_BIT_LENGTH];
	unsigned int history;
	// Return pointer stack
	unsigned int ras[RAS_LENGTH];
	unsigned int ras_pointer;
	my_update update_info;
	branch_info bi;

	my_predictor(void) : history(0)
	{
		memset(map, 1, sizeof(map));
		memset(ras, 0, sizeof(ras));
	}

	branch_update *predict(branch_info &info)
	{
		bi = info;
		if (info.br_flags & BR_CONDITIONAL)
		{
			update_info.index = (info.address ^ history) & ((1 << N_BIT_LENGTH) - 1);
			update_info.direction_prediction(map[update_info.index] >> 1);
		}
		else if (info.br_flags & BR_RETURN)
		{
			if (info.address == ras[ras_pointer])
			{
				update_info.direction_prediction(true);
				ras_pointer -= 1;
			}
			else
			{
				update_info.direction_prediction(false);
			}
		}
		else if (info.br_flags & BR_CALL)
		{
			ras[ras_pointer] = info.address;
			update_info.direction_prediction(true);
		}
		else
		{
			update_info.direction_prediction(true);
		}
		update_info.target_prediction(0);
		// wrap around when pointer > ras length - 1
		if (ras_pointer > RAS_LENGTH - 1)
		{
			ras_pointer = 0;
		}
		return &update_info;
	}

	void update(branch_update *update, bool taken, unsigned int target)
	{
		if (bi.br_flags & BR_CONDITIONAL)
		{
			my_update* casted_update = (my_update *)update;
			unsigned char *position = &map[casted_update->index];
			unsigned char prediction = map[casted_update->index];
			if (taken)
			{
				if (prediction < 3)
					(*position)++;
			}
			else
			{
				if (prediction > 0)
					(*position)--;
			}
			history <<= 1;
			history |= taken;
		}
		else if (bi.br_flags & BR_RETURN)
		{
			if (!taken)
			{
				ras_pointer += 1;
			}
		}
		else if (bi.br_flags & BR_CALL)
		{
			if (!taken)
			{
				ras_pointer -= 1;
			}
		}
		if (ras_pointer > RAS_LENGTH - 1)
		{
			ras_pointer = 0;
		}
	}
};
