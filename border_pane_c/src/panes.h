/*
 * panes.h
 *
 * A `pane` is a collection of one or more ncplanes with a
 * predefined ncplane_resizecb.
 */

#ifndef PANES_H
#define PANES_H

#include <notcurses/notcurses.h>
#include <wchar.h>

#define MALLOC_ERROR -1
#define TPLANE_SCALE 1.0 / 8.0
#define BPLANE_SCALE 1.0 / 5.0
#define LR_SCALE 1.0 / 6.0

/*
 * notcurses instance
 */
extern struct notcurses *nc;

/*
 * Border Pane
 *
 * |--------------|
 * |--------------|
 * |  |        |  |
 * |  |        |  |
 * |  |        |  |
 * |--------------|
 * |--------------|
 *
 * A pane composed of a left, right, top, bottom and center plane.
 */
struct border_pane {
  struct ncplane *self, *lplane, *rplane, *cplane, *tplane, *bplane;
};

struct border_pane *bp_create(struct ncplane *parent);

/*
 * Set rounder perimeter for every child plane in the border_pane.
 */
void bp_perimeter_rounded(struct border_pane *bp, uint16_t stylemask,
                          uint64_t channels, unsigned int ctlword);

/*
 * Border pane equivalent of `ncplane_erase`.
 */
void bp_erase(struct border_pane *bp);

/*
 * Text-containing pane
 */
struct text_pane {
  const char *text;
  struct ncplane *self;
};

struct text_pane *tp_create(const char *text, struct ncplane *parent);

#endif // !PANES_H
