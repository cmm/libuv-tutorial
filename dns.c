#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <uv.h>
#include "co.h"

uv_loop_t *loop;

typedef struct {
  struct addrinfo hints;
  uv_getaddrinfo_t resolver;
  char addr[17];
  uv_connect_t connect_req;
  uv_tcp_t socket;
  char buffer[32];
  uv_buf_t w_buf;
  uv_write_t write_req;
  uv_stream_t *stream;
} state_t;
co_define(foo, co_none_t, co_none_t, state_t, NULL);
void foo_co(co_base_t *co) {
  co_prologue(foo, co, _, state);
  state->hints = (typeof(state->hints)){
    .ai_family = PF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP,
    .ai_flags = 0
  };
  fprintf(stderr, "irc.freenode.net is ");
  uv_await(ret, getaddrinfo, &state->resolver, "irc.freenode.net", "6667", &state->hints);
  if (co_errno) {
    fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(co_errno));
    co_return({});
  }
  if (ret->status < 0) {
    fprintf(stderr, "getaddrinfo error %s\n", uv_err_name(ret->status));
    co_return({});
  }
  uv_ip4_name((struct sockaddr_in*)ret->res->ai_addr, state->addr, 16);
  fprintf(stderr, "%s\n", state->addr);
  uv_tcp_init(co->loop, &state->socket);
  __auto_type ai_addr = (const struct sockaddr*)ret->res->ai_addr;
  uv_await(c, tcp_connect, &state->connect_req, &state->socket, ai_addr);
  if (c->status < 0) {
    fprintf(stderr, "connect error\n");
    co_return({});
  }
  static char msg[] = "hello";
  state->w_buf = (uv_buf_t){.len = strlen(msg), .base = msg};
  __auto_type tcp = (uv_stream_t *)c->req->handle;
  uv_await(w, write, &state->write_req, tcp, &state->w_buf, 1);
  if (w->status < 0) {
    fprintf(stderr, "write error");
    co_return({});
  }
  state->stream = w->req->handle;
  while (true) {
    uv_await(r, read, state->stream, (uv_buf_t){.len = sizeof(state->buffer) - 1, .base = state->buffer});
    if (r->nread == UV_EOF) {
      printf("\n");
      break;
    }
    if (r->nread < 0) {
      fprintf(stderr, "read error");
      co_return({});
    }
    r->buf->base[r->nread] = '\0';
    printf("%s", r->buf->base);
  }

  uv_await0(close, (uv_handle_t *)state->stream);
  co_epilogue({});
}

int main() {
  loop = uv_default_loop();

  co_launch(loop, NULL, foo, {});
  return uv_run(loop, UV_RUN_DEFAULT);
}
