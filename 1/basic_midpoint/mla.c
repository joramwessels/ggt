/* Computer Graphics, Assignment 1, Bresenham's Midpoint Line-Algorithm
 *
 * Filename ........ mla.c
 * Description ..... Midpoint Line Algorithm
 * Created by ...... Jurgen Sturm 
 *
 * Student name .... Joram Wessels				Wouter Lont
 * Student email ... joram.wessels@outlook.com	-
 * Collegekaart .... 10631542					-
 * Date ............ 03-02-2016
 * Comments ........ No
 */

#include "SDL.h"   
#include "init.h"

/* Function: octant
 * -------------------------------------
 * Determines in which octant a line is.
 * 
 * x0: The x-coordinate of the start of the line.
 * y0: The y-coordinate of the start of the line.
 * x1: The x-coordinate of the end of the line.
 * y1: The y-coordinate of the end of the line.
 * 
 * returns: An integer indicator of the octant, ranging from
 * 			1 to 8, going counter clockwise around the axis.
 */
int octant(int x0, int y0, int x1, int y1) {
	int dx = x1 - x0, dy = y1 - y0, base;
	if (dx > 0 && dy >= 0) {
		base = 1;
	} else if (dx <= 0 && dy > 0) {
		base = 3;
		dy = -dy;
	} else if (dx < 0 &&dy <= 0) {
		base = 5;
		dx = -dx;
		dy = -dy;
	} else if (dx >= 0 && dy < 0) {
		base = 7;
		dx = -dx;
	}
	return base + (((dx - dy) & 0x8000) >> 15);
}

/* Function: draw_line
 * ------------------------
 * Draws a line on a surface.
 * 
 * *s:		 The pointer to a SDL_Surface to draw on.
 * colour:	 The Uint32 struct specifying the colour.
 * iterator: The distance in the iterated dimension.
 * target:	 The distance in the conditionally incremented dimension.
 * it_incr:	 The amount 'it' should increase each iteration.
 * tg_incr:	 The amount 'tg' should increase each midpoint junction.
 * it0:		 The iterator coordinate of the start of the line.
 * tg0:		 The target coordinate of the start of the line.
 * itIsX:	 An integer that indicates if 'it' is the x dimension.
 *
 */
void draw_line(SDL_Surface *s, Uint32 colour, int iterator, int target,
				int it_incr, int tg_incr, int it0, int tg0, int itIsX) {
	int i, tg = tg0, it = it0, d = iterator / 2;
	for (i=0; i<iterator; i++) {
		if (itIsX) {
			PutPixel(s, it, tg, colour);
		} else {
			PutPixel(s, tg, it, colour);
		}
		d += target;
		if (d < iterator) {
			it += it_incr;
		} else {
			it += it_incr;
			tg += tg_incr;
			d -= iterator;
		}
	}
}

/* Function: mla
 * -------------------------------------------------------------
 * Draws a line on a surface, regardless of the octant it is in.
 * 
 * *s: The pointer to a SDL_Surface on which to draw.
 * x0: The x-coordinate of the start of the line.
 * y0: The y-coordinate of the start of the line.
 * x1: The x-coordinate of the end of the line.
 * y1: The y-coordinate of the end of the line.
 * c:  The Uint32 struct specifying the colour.
 *
 */
void mla(SDL_Surface *s, int x0, int y0, int x1, int y1, Uint32 c) {
	int o = octant(x0, y0, x1, y1);
	printf("octant:%d  %d:%d  %d:%d", o, x0, y0, x1, y1);
	int dx = x1 - x0, dy = y1 - y0;
	int it_incr = 1, tg_incr = 1;
	if (dy < 0) it_incr = -1;
	if (dx < 0) tg_incr = -1;
	if (abs(dx) > abs(dy)) {
		draw_line(s, c, abs(dx), abs(dy), tg_incr, it_incr, x0, y0, 1);
	} else {
		draw_line(s, c, abs(dy), abs(dx, it_incr, tg_incr, y0, x0, 0)
	}
	return;
}