/*
 * panes.c
 *
 * Implementation of pane types declared in panes.h
 */

#include "panes.h"

#include <math.h>
#include <notcurses/notcurses.h>

/*
 * Generic plane that includes a left, center and right plane
 */
struct tri_plane {
  struct ncplane *self, *lplane, *rplane, *cplane;
};

/*
 * Plane creation auxiliary methods
 */
struct ncplane *lplane_create(struct ncplane *parent) {

  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const struct ncplane_options lopts = {
      .y = 0, .x = 0, .rows = height_p, .cols = width_p * LR_SCALE};

  struct ncplane *left = ncplane_create(parent, &lopts);

  return left;
}

struct ncplane *rplane_create(struct ncplane *parent) {

  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int rplane_x_start = ceil(width_p * (1 - LR_SCALE));

  const struct ncplane_options ropts = {.y = 0,
                                        .x = rplane_x_start,
                                        .rows = height_p,
                                        .cols = width_p - rplane_x_start};

  struct ncplane *right = ncplane_create(parent, &ropts);

  return right;
}

struct ncplane *cplane_create(struct ncplane *parent) {

  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int cplane_x_start = LR_SCALE * width_p;
  const unsigned cplane_cols = width_p - 2 * cplane_x_start;

  const struct ncplane_options copts = {
      .y = 0, .x = cplane_x_start, .rows = height_p, .cols = cplane_cols};

  struct ncplane *center = ncplane_create(parent, &copts);

  return center;
}

struct tri_plane *tri_plane_create(struct ncplane *parent) {

  unsigned width, height;
  ncplane_dim_yx(parent, &height, &width);

  const struct ncplane_options cr_opts = {
      .y = 0, .x = 0, .rows = height, .cols = width};

  struct ncplane *cr = ncplane_create(parent, &cr_opts);
  struct ncplane *left = lplane_create(cr), *right = rplane_create(cr),
                 *center = cplane_create(cr);

  struct tri_plane *tri = (struct tri_plane *)malloc(sizeof(struct tri_plane));
  if (tri == NULL) {
    perror("[ERROR] malloc malfunction in 'tri_plane_create'");
    notcurses_stop(nc);
    exit(MALLOC_ERROR);
  }

  tri->self = cr;
  tri->lplane = left;
  tri->rplane = right;
  tri->cplane = center;

  return tri;
}

struct ncplane *tplane_create(struct ncplane *parent) {
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const struct ncplane_options topts = {
      .y = 0, .x = 0, .rows = height_p * TPLANE_SCALE, .cols = width_p};
  struct ncplane *top = ncplane_create(parent, &topts);

  return top;
}

struct ncplane *bplane_create(struct ncplane *parent) {
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int bplane_y_start = ceil(height_p * (1 - BPLANE_SCALE));
  const struct ncplane_options bopts = {.y = bplane_y_start,
                                        .x = 0,
                                        .rows = height_p - bplane_y_start,
                                        .cols = width_p};
  struct ncplane *bottom = ncplane_create(parent, &bopts);

  return bottom;
}

/*
 * Resize methods
 */
int resize_simple(struct ncplane *n) {
  struct ncplane *parent = ncplane_parent(n);
  unsigned rows, cols;
  ncplane_dim_yx(parent, &rows, &cols);
  ncplane_resize_simple(n, rows, cols);
  ncplane_erase(n);
  return 0;
}

int resize_lplane(struct ncplane *lp) {
  const struct ncplane *parent = ncplane_parent(lp);
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  ncplane_resize_simple(lp, height_p, width_p * LR_SCALE);
  ncplane_erase(lp);
  return 0;
}

int resize_rplane(struct ncplane *rp) {
  const struct ncplane *parent = ncplane_parent(rp);
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int rplane_x_start = ceil(width_p * (1 - LR_SCALE));
  ncplane_resize_simple(rp, height_p, width_p - rplane_x_start);
  ncplane_move_yx(rp, 0, rplane_x_start);
  ncplane_erase(rp);
  return 0;
}

int resize_cplane(struct ncplane *cp) {
  const struct ncplane *parent = ncplane_parent(cp);
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int cplane_x_start = LR_SCALE * width_p;
  const unsigned cplane_cols = width_p - 2 * cplane_x_start;
  ncplane_resize_simple(cp, height_p, cplane_cols);
  ncplane_move_yx(cp, 0, cplane_x_start);
  ncplane_erase(cp);
  return 0;
}

int resize_tplane(struct ncplane *tp) {
  const struct ncplane *parent = ncplane_parent(tp);
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  ncplane_resize_simple(tp, height_p * TPLANE_SCALE, width_p);
  ncplane_erase(tp);
  return 0;
}

int resize_bplane(struct ncplane *bp) {
  const struct ncplane *parent = ncplane_parent(bp);
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int bplane_y_start = ceil(height_p * (1 - BPLANE_SCALE));
  ncplane_resize_simple(bp, height_p - bplane_y_start, width_p);
  ncplane_move_yx(bp, bplane_y_start, 0);
  ncplane_erase(bp);
  return 0;
}

int resize_center_band(struct ncplane *cb) {
  const struct ncplane *parent = ncplane_parent(cb);
  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int center_top = TPLANE_SCALE * height_p;
  const unsigned center_height =
      ceil(height_p - (height_p * (TPLANE_SCALE + BPLANE_SCALE)));
  ncplane_resize_simple(cb, center_height, width_p);
  ncplane_move_yx(cb, center_top, 0);
  ncplane_erase(cb);
  return 0;
}

/*
 * Border pane implementation
 */
struct border_pane *bp_create(struct ncplane *parent) {

  unsigned width_p, height_p;
  ncplane_dim_yx(parent, &height_p, &width_p);

  const int center_top = TPLANE_SCALE * height_p;
  const unsigned center_height =
      ceil(height_p - (height_p * (TPLANE_SCALE + BPLANE_SCALE)));

  const struct ncplane_options border_pane_opts = {0, 0, height_p, width_p},
                               center_band_opts = {center_top, 0, center_height,
                                                   width_p};

  struct ncplane *border_pane = ncplane_create(parent, &border_pane_opts),
                 *center_band = ncplane_create(border_pane, &center_band_opts),
                 *tplane = tplane_create(border_pane),
                 *bplane = bplane_create(border_pane);

  struct tri_plane *tri = tri_plane_create(center_band);

  struct border_pane *bp =
      (struct border_pane *)malloc(sizeof(struct border_pane));
  if (bp == NULL) {
    perror("[ERROR] malloc malfunction in 'bp_create'");
    notcurses_stop(nc);
    exit(MALLOC_ERROR);
  }

  bp->self = border_pane;
  bp->lplane = tri->lplane;
  bp->rplane = tri->rplane;
  bp->cplane = tri->cplane;
  bp->tplane = tplane;
  bp->bplane = bplane;

  ncplane_set_resizecb(border_pane, resize_simple);
  ncplane_set_resizecb(center_band, resize_center_band);
  ncplane_set_resizecb(tri->self, resize_simple);
  ncplane_set_resizecb(bp->lplane, resize_lplane);
  ncplane_set_resizecb(bp->rplane, resize_rplane);
  ncplane_set_resizecb(bp->cplane, resize_cplane);
  ncplane_set_resizecb(bp->tplane, resize_tplane);
  ncplane_set_resizecb(bp->bplane, resize_bplane);

  free(tri);

  return bp;
}

void bp_perimeter_rounded(struct border_pane *bp, uint16_t stylemask,
                          uint64_t channels, unsigned int ctlword) {
  ncplane_perimeter_rounded(bp->lplane, stylemask, channels, ctlword);
  ncplane_perimeter_rounded(bp->rplane, stylemask, channels, ctlword);
  ncplane_perimeter_rounded(bp->tplane, stylemask, channels, ctlword);
  ncplane_perimeter_rounded(bp->bplane, stylemask, channels, ctlword);
  ncplane_perimeter_rounded(bp->cplane, stylemask, channels, ctlword);
}

void bp_erase(struct border_pane *bp) {
  ncplane_erase(bp->lplane);
  ncplane_erase(bp->rplane);
  ncplane_erase(bp->tplane);
  ncplane_erase(bp->bplane);
  ncplane_erase(bp->cplane);
}

int resize_tp(struct ncplane *tp) {
  struct ncplane *parent = ncplane_parent(tp);
  unsigned rows, cols;
  ncplane_dim_yx(parent, &rows, &cols);
  ncplane_resize_simple(tp, rows, cols);
  ncplane_erase(tp);

  if (ncplane_userptr(tp) == NULL)
    return 1;

  const char *text = (const char *)ncplane_userptr(tp);
  ncplane_puttext(tp, ncplane_dim_y(parent) / 2, NCALIGN_CENTER, text, NULL);

  return 0;
}

struct text_pane *tp_create(const char *text, struct ncplane *parent) {
  struct text_pane *tp = (struct text_pane *)malloc(sizeof(struct text_pane));
  if (tp == NULL) {
    perror("[ERROR] malloc malfunction in 'tp_create'");
    notcurses_stop(nc);
    exit(MALLOC_ERROR);
  }

  struct ncplane_options opts = {0, 0, ncplane_dim_y(parent),
                                 ncplane_dim_x(parent)};
  struct ncplane *plane = ncplane_create(parent, &opts);

  tp->self = plane;
  tp->text = text;

  ncplane_set_userptr(tp->self, (void *)tp->text);
  ncplane_puttext(tp->self, ncplane_dim_y(parent) / 2, NCALIGN_CENTER, tp->text,
                  NULL);

  ncplane_set_resizecb(tp->self, resize_tp);

  return tp;
}
