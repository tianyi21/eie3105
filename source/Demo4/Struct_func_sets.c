//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

//	Declare in main.c
extern struct track_struct track;
extern struct loc_struct object[4];
extern struct kick_struct kick;

loc_struct read_loc(obj sel)
{
	return object[sel];
}

void clear_loc(obj sel)
{
	object[sel].X = 0;
	object[sel].Y = 0;
}

void update_loc(loc_Update_struct val)
{
	object[val.object].X = val.X;
	object[val.object].Y = val.Y;
	
	if (val.object == BALL)
	{
		object[TARGET].X = val.X + track.anchor_x - 5;
		object[TARGET].Y = val.Y + track.anchor_y;
		if (object[TARGET].X < 0)
			object[TARGET].X = 0;
		if (object[TARGET].Y < 0)
			object[TARGET].Y = 0;
	}
}

loc_struct cal_diff(obj sel_1, obj sel_2)
{
	loc_struct diff;
	diff.X = object[sel_1].X - object[sel_2].X;
	diff.Y = object[sel_1].Y - object[sel_2].Y;
	
	return diff;
}

int cal_dist(obj sel_1, obj sel_2)
{
	loc_struct diff = cal_diff(sel_1, sel_2);
	int dist = (int)sqrt((double)(diff.X * diff.X) + (double)(diff.Y * diff.Y));
	
	return dist;
}

int cal_dist_struct(loc_struct sel_1, loc_struct sel_2)
{
	int dist = (int)sqrt((double)((sel_2.X - sel_1.X) * (sel_2.X - sel_1.X)) + (double)((sel_2.Y - sel_1.Y) * (sel_2.Y - sel_1.Y)));
	
	return dist;
}


