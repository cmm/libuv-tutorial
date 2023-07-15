#include <stdio.h>
#include <uv.h>
#include "co.h"

co_define(
    idler, (), int,
    {
      int64_t counter;
      uv_idle_t idler;
    },
    {
      co_prologue;
      vars->counter = 0;
      uv_idle_init(_co->loop, &vars->idler);
      uv_await0(idle_start, &vars->idler);
      do {
        printf("c=%lu\n", vars->counter);
      } while (++vars->counter < 10);
      uv_idle_stop(&vars->idler);
      co_epilogue(0);
    });

int main() {
  co_t idle_co;
  printf("Idling...\n");
  co_launch(&idle_co, uv_default_loop(), idler);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  uv_loop_close(uv_default_loop());
  return 0;
}
