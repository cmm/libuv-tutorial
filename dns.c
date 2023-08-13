#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <uv.h>
#include "co.h"

typedef struct {
  uv_getaddrinfo_t resolver;
  struct addrinfo *ai;
  uv_connect_t connect_req;
  uv_tcp_t socket;
  char buffer[32];
  uv_write_t write_req;
  uv_stream_t *stream;
  bool have_stream;
  uv_out_t uv;
} state_t;
co(foo, co_none_t, co_none_t, state_t) {
  co_begin(_, s);

  static const struct addrinfo hints = {
    .ai_family = PF_INET,
    .ai_socktype = SOCK_STREAM,
    .ai_protocol = IPPROTO_TCP,
    .ai_flags = 0
  };
  fprintf(stderr, "irc.freenode.net is ");
  uv_await(&s->uv.getaddrinfo, getaddrinfo, &s->resolver, "irc.freenode.net", "6667", &hints);
  if (co_status) {
    fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(co_status));
    co_return({});
  }
  if (s->uv.getaddrinfo.status < 0) {
    fprintf(stderr, "getaddrinfo error %s\n", uv_err_name(s->uv.getaddrinfo.status));
    co_return({});
  }
  s->ai = s->uv.getaddrinfo.res;
  char addr[17];
  uv_ip4_name((struct sockaddr_in *)s->ai->ai_addr, addr, 16);
  fprintf(stderr, "%s\n", addr);

  uv_tcp_init(co_loop, &s->socket);
  uv_await(&s->uv.connect, tcp_connect, &s->connect_req, &s->socket, (const struct sockaddr *)s->ai->ai_addr);
  if (co_status || s->uv.connect.status < 0) {
    fprintf(stderr, "connect error\n");
    co_return({});
  }

  static char msg[] = "hello";
  __auto_type w_buf = (uv_buf_t){.len = strlen(msg), .base = msg};
  uv_await(&s->uv.write, write, &s->write_req, (uv_stream_t *)s->uv.connect.req->handle, &w_buf, 1);
  if (co_status || s->uv.write.status < 0) {
    fprintf(stderr, "write error");
    co_return({});
  }
  s->stream = s->uv.write.req->handle;

  while (true) {
    uv_await(&s->uv.read, read, s->stream, (uv_buf_t){.len = sizeof(s->buffer) - 1, .base = s->buffer});
    if (co_status || s->uv.read.nread == UV_EOF) {
      printf("\n");
      break;
    }
    s->have_stream = true;
    if (s->uv.read.nread < 0) {
      fprintf(stderr, "read error");
      co_return({});
    }
    s->uv.read.buf->base[s->uv.read.nread] = '\0';
    printf("%s", s->uv.read.buf->base);
  }

  co_end_with_cleanup({});
  if (s->have_stream)
    uv_await(NULL, close, (uv_handle_t *)s->stream);
  uv_freeaddrinfo(s->ai);
  co_cleanup_end;
}

int main() {
  __auto_type loop = uv_default_loop();

  co_launch(loop, NULL, foo, {});
  int ret = uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
  return ret;
}
