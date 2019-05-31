//  Ⓒ Copyright Tianyi Liu 2019, All Rights Reserved.
//  For reference only.
//  If you use or partially use this repo, you shall formally acknowledge this repo.
//  Latest Update: 15:32 May 31st, 2019 CST

#include "header.h"

//	Declare in main.c
extern struct loc_struct object[6];

loc_struct read_loc(obj sel)
{
	/*	Usage:
	//		INPUT:	
	//			sel: obj enum variable
	//		RETURN:
	//			selected object: car, ball_1, ball_2, ball_3
	//
	*/
	
	return object[sel];
}

void clear_loc(obj sel)
{
	/*	Usage:
	//		INPUT:
	//			sel: obj enum variable
	//
	*/
	
	object[sel].X = 0;
	object[sel].Y = 0;
}

void update_loc(loc_Update_struct val)
{
	/*	Usage:
	//		INPUT:
	//			val: loc_update enum variable
	//				val.object:	select object
	//				val.x:			new x axis
	//				val.y:			new y axis
	//
	*/
	
	object[val.object].X = val.X;
	object[val.object].Y = val.Y;
}

loc_struct cal_diff(obj sel_1, obj sel_2)
{
	/*
	//	Usage:
	//		INPUT:
	//			sel_1: obj enum, object 1
	//			sel_2: obj enum, object 2
	//		RETURN:
	//			loc object contains delta x, y
	//
	*/
	loc_struct diff;
	diff.X = object[sel_1].X - object[sel_2].X;
	diff.Y = object[sel_1].Y - object[sel_2].Y;
	
	return diff;
}

int cal_dist(obj sel_1, obj sel_2)
{
	/*
	//	Usage:
	//		INPUT:
	//			sel_1: obj enum, object 1
	//			sel_2: obj enum, object 2
	//		RETURN:
	//			int dist: Euclidean Dist
	*/
	
	loc_struct diff = cal_diff(sel_1, sel_2);
	int dist = sqrt((double)(diff.X * diff.X) + (double)(diff.Y * diff.Y));
	
	return dist;
}




