#include "panes.h"
#include <locale.h>
#include <notcurses/notcurses.h>
#include <stdlib.h>

void keep_stable_framerate() {
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 1667};
  nanosleep(&ts, NULL);
}

typedef enum { SHOW_BORDERS, NO_BORDERS } border_s;

/* declaration in panes.h */
struct notcurses *nc = NULL;
int main() {

  /*
   * Notcurses init
   */
  setlocale(LC_ALL, "");
  notcurses_options opts = {0};

  nc = notcurses_core_init(&opts, stdout);

  struct ncplane *std = notcurses_stdplane(nc);
  struct border_pane *bp = bp_create(std);

  const char *top_txt = "Top", *bottom_txt = "Bottom", *left_txt = "Left",
             *right_txt = "Right",
             *instructions = "Center\n\nPress 'b' to toggle borders ON/OFF";

  struct text_pane *top_txt_pane = tp_create(top_txt, bp->tplane),
                   *bottom_txt_pane = tp_create(bottom_txt, bp->bplane),
                   *left_txt_pane = tp_create(left_txt, bp->lplane),
                   *right_txt_pane = tp_create(right_txt, bp->rplane),
                   *instr_pane = tp_create(instructions, bp->cplane);

  /*
   * Main loop
   */
  struct ncinput input;
  uint32_t key;
  border_s border_status = NO_BORDERS;
  struct timespec delay = {1, 0};
  while ((key = notcurses_get(nc, &delay, &input)) != (uint32_t)-1) {

    if (key == 'q' && input.evtype != NCTYPE_RELEASE) {
      notcurses_stop(nc);
      exit(0);
    }

    if (key == 'b' && input.evtype != NCTYPE_RELEASE)
      border_status = border_status == SHOW_BORDERS ? NO_BORDERS : SHOW_BORDERS;

    switch (border_status) {
    case SHOW_BORDERS: {
      bp_perimeter_rounded(bp, 0, 0, 0);
    } break;
    case NO_BORDERS: {
      bp_erase(bp);
    } break;
    }

    notcurses_render(nc);
    keep_stable_framerate();
  }

  ncplane_destroy(bp->self);
  free(top_txt_pane);
  free(bottom_txt_pane);
  free(left_txt_pane);
  free(right_txt_pane);
  free(instr_pane);
  free(bp);

  notcurses_stop(nc);
  return 0;
}
