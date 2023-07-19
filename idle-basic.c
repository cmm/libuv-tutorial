#include <stdio.h>
#include <uv.h>
#include "co.h"

co_define(idle, uv_idle_t *, int, int64_t, NULL);
void idle_co(co_base_t *co) {
  co_prologue(idle, co);
  *state = 0;
  uv_idle_init(co->loop, *in);
  do {
    uv_await0(idle_start, *in);
    printf("c=%lu\n", *state);
    uv_idle_stop(*in);
  } while (++*state < 10);
  co_epilogue(0);
}

int main() {
  uv_idle_t idler;
  __auto_type loop = uv_default_loop();
  printf("Idling...\n");
  co_launch(loop, NULL, idle, &idler);
  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
  return 0;
}
