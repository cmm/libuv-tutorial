#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <uv.h>

struct co;
typedef void (_co_fn_t)(struct co *, void *promise, void *args, void *vars);

#define _co_count(...)                                                         \
  _co_count_(_, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,   \
             3, 2, 1, 0)

#define _co_count_(_, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,  \
                   _14, _15, _16, N, ...)                                      \
  N

#define _co_fields_0()
#define _co_fields_1(X) X;
#define _co_fields_2(X, ...) X; _co_fields_1(__VA_ARGS__)
#define _co_fields_3(X, ...) X; _co_fields_2(__VA_ARGS__)
#define _co_fields_4(X, ...) X; _co_fields_3(__VA_ARGS__)
#define _co_fields_5(X, ...) X; _co_fields_4(__VA_ARGS__)
#define _co_fields_6(X, ...) X; _co_fields_5(__VA_ARGS__)
#define _co_fields_7(X, ...) X; _co_fields_6(__VA_ARGS__)
#define _co_fields_8(X, ...) X; _co_fields_7(__VA_ARGS__)
#define _co_fields_9(X, ...) X; _co_fields_8(__VA_ARGS__)
#define _co_fields_10(X, ...) X; _co_fields_9(__VA_ARGS__)
#define _co_fields_11(X, ...) X; _co_fields_10(__VA_ARGS__)
#define _co_fields_12(X, ...) X; _co_fields_11(__VA_ARGS__)
#define _co_fields_13(X, ...) X; _co_fields_12(__VA_ARGS__)
#define _co_fields_14(X, ...) X; _co_fields_13(__VA_ARGS__)
#define _co_fields_15(X, ...) X; _co_fields_14(__VA_ARGS__)
#define _co_fields_16(X, ...) X; _co_fields_15(__VA_ARGS__)
#define _co_fields(N, ...) _co_fields_(N, ##__VA_ARGS__)
#define _co_fields_(N, ...) _co_fields_##N(__VA_ARGS__)
#define _co_struct_body(...)                                                   \
  { _co_fields(_co_count(__VA_ARGS__), ##__VA_ARGS__) }

typedef struct co {
  uv_loop_t* loop;
  _co_fn_t *fn;
  void *promise;
  void *args;
  void *vars;
  int line;
  struct {
    void *promise;
    struct co *co;
  } nested;
} co_t;

static inline __attribute__((unused))
void _co_destroy(co_t *co) {
  free(co->vars);
  free(co->args);
  free(co->nested.promise);
  free(co->nested.co);
}

#define co_declare(ATTRS, NAME, ARGS, RESULT_TYPE)                             \
  typedef struct _co_struct_body ARGS NAME##__args_t;                          \
  typedef struct {                                                             \
    co_t *waiter;                                                              \
    RESULT_TYPE result;                                                        \
  } NAME##__promise_t;                                                         \
  struct NAME##__vars;                                                         \
  ATTRS void NAME##__co(co_t *, NAME##__promise_t *, NAME##__args_t *,         \
                        struct NAME##__vars *)

#define co_implement(NAME, VAR_STRUCT_BODY, CODE)                              \
  typedef struct NAME##__vars VAR_STRUCT_BODY NAME##__vars_t;                  \
  void NAME##__co(co_t *_co, NAME##__promise_t *_promise,                      \
                  NAME##__args_t *args, NAME##__vars_t *vars) CODE

#define co_define(NAME, ARGS, RESULT_TYPE, VAR_STRUCT_BODY, CODE)              \
  co_declare(static, NAME, ARGS, RESULT_TYPE);                                 \
  co_implement(NAME, VAR_STRUCT_BODY, CODE)

#define co_return(RESULT)                                                      \
  do {                                                                         \
    _co_destroy(_co);                                                          \
    if (_promise) {                                                            \
      _promise->result = RESULT;                                               \
      co_t *_waiter = _promise->waiter;                                        \
      _waiter->fn(_waiter, _waiter->promise, _waiter->args, _waiter->vars);    \
    }                                                                          \
    return;                                                                    \
  } while (false)

#define _co_launch(CO, PROMISE, LOOP, NAME, ...)                               \
  do {                                                                         \
    co_t *_co = CO;                                                            \
    _co->loop = LOOP;                                                          \
    _co->fn = (_co_fn_t *)NAME##__co;                                          \
    _co->promise = PROMISE;                                                    \
    if (sizeof(NAME##__args_t)) {                                              \
      const NAME##__args_t _args_ = {__VA_ARGS__};                             \
      NAME##__args_t *_args = malloc(sizeof(NAME##__args_t));                  \
      *_args = _args_;                                                         \
      _co->args = _args;                                                       \
    } else {                                                                   \
      _co->args = NULL;                                                        \
    }                                                                          \
    _co->line = 0;                                                             \
    _co->vars = NULL;                                                          \
    _co->nested.promise = NULL;                                                \
    _co->nested.co = NULL;                                                     \
    _co->fn(_co, _co->promise, _co->args, _co->vars);                          \
  } while (false)

#define co_launch(CO, LOOP, NAME, ...)                                         \
  _co_launch(CO, NULL, LOOP, NAME, ##__VA_ARGS__)

#define co_prologue                                                            \
  int co_errno = 0;                                                            \
  switch (_co->line) {                                                         \
  case 0:                                                                      \
    if (sizeof(typeof(*vars)) > 0)                                             \
      vars = _co->vars = calloc(sizeof(typeof(*vars)), 1)

#define co_epilogue(RESULT)                                                    \
  }                                                                            \
  co_return(RESULT)

#define co_await0(NAME, ...)                                                   \
  do {                                                                         \
    if (!_co->nested.co)                                                       \
      _co->nested.co = malloc(sizeof(co_t));                                   \
    _co->nested.promise =                                                      \
        realloc(_co->nested.promise, sizeof(NAME##__promise_t));               \
    _co->nested.promise->waiter = _co;                                         \
    _co->line = __LINE__;                                                      \
    _co_launch(_co->nested.co, _co->nested.promise, _co->loop, NAME,           \
               ##__VA_ARGS__);                                                 \
    return;                                                                    \
  case __LINE__:;                                                              \
  } while (false)


#define co_await(RES_VAR, NAME, ...)                                           \
  co_await0(NAME, ##__VA_ARGS__);                                              \
  __auto_type RES_VAR = &((NAME##__promise_t *)_co->nested.promise)->result

// *** UV ***
#define _co_tn_fields_0()
#define _co_tn_fields_2(TYPE, NAME) TYPE NAME;
#define _co_tn_fields_4(TYPE, NAME, ...)                                       \
  TYPE NAME;                                                                   \
  _co_tn_fields_2(__VA_ARGS__)
#define _co_tn_fields_6(TYPE, NAME, ...)                                       \
  TYPE NAME;                                                                   \
  _co_tn_fields_4(__VA_ARGS__)
#define _co_tn_fields_8(TYPE, NAME, ...)                                       \
  TYPE NAME;                                                                   \
  _co_tn_fields_6(__VA_ARGS__)
#define _co_tn_fields_10(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_8(__VA_ARGS__)
#define _co_tn_fields_12(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_10(__VA_ARGS__)
#define _co_tn_fields_14(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_12(__VA_ARGS__)
#define _co_tn_fields_16(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_14(__VA_ARGS__)
#define _co_tn_fields(N, ...) _co_tn_fields_(N, ##__VA_ARGS__)
#define _co_tn_fields_(N, ...) _co_tn_fields_##N(__VA_ARGS__)
#define _co_tn_struct_body(...)                                                \
  { _co_tn_fields(_co_count(__VA_ARGS__), ##__VA_ARGS__) }

#define _co_tn_args_0()
#define _co_tn_args_2(TYPE, NAME) TYPE NAME
#define _co_tn_args_4(TYPE, NAME, ...) TYPE NAME, _co_tn_args_2(__VA_ARGS__)
#define _co_tn_args_6(TYPE, NAME, ...) TYPE NAME, _co_tn_args_4(__VA_ARGS__)
#define _co_tn_args_8(TYPE, NAME, ...) TYPE NAME, _co_tn_args_6(__VA_ARGS__)
#define _co_tn_args_10(TYPE, NAME, ...) TYPE NAME, _co_tn_args_8(__VA_ARGS__)
#define _co_tn_args_12(TYPE, NAME, ...) TYPE NAME, _co_tn_args_10(__VA_ARGS__)
#define _co_tn_args_14(TYPE, NAME, ...) TYPE NAME, _co_tn_args_12(__VA_ARGS__)
#define _co_tn_args_16(TYPE, NAME, ...) TYPE NAME, _co_tn_args_14(__VA_ARGS__)
#define _co_tn_args(N, ...) _co_tn_args_(N, ##__VA_ARGS__)
#define _co_tn_args_(N, ...) _co_tn_args_##N(__VA_ARGS__)
#define _co_tn_arglist(...) (_co_tn_args(_co_count(__VA_ARGS__), ##__VA_ARGS__))

#define _co_tn_names_0()
#define _co_tn_names_2(_, NAME) NAME
#define _co_tn_names_4(_, NAME, ...) NAME, _co_tn_names_2(__VA_ARGS__)
#define _co_tn_names_6(_, NAME, ...) NAME, _co_tn_names_4(__VA_ARGS__)
#define _co_tn_names_8(_, NAME, ...) NAME, _co_tn_names_6(__VA_ARGS__)
#define _co_tn_names_10(_, NAME, ...) NAME, _co_tn_names_8(__VA_ARGS__)
#define _co_tn_names_12(_, NAME, ...) NAME, _co_tn_names_10(__VA_ARGS__)
#define _co_tn_names_14(_, NAME, ...) NAME, _co_tn_names_12(__VA_ARGS__)
#define _co_tn_names_16(_, NAME, ...) NAME, _co_tn_names_14(__VA_ARGS__)
#define _co_tn_names(N, ...) _co_tn_names_(N, ##__VA_ARGS__)
#define _co_tn_names_(N, ...) _co_tn_names_##N(__VA_ARGS__)
#define _co_tn_initform(...)                                                   \
  { _co_tn_names(_co_count(__VA_ARGS__), ##__VA_ARGS__) }

#define _co_define_uv_cb(NAME, HorR_TYPE, HorR_NAME, ...)                      \
  typedef struct {                                                             \
    co_t *waiter;                                                              \
    struct _co_tn_struct_body(HorR_TYPE, HorR_NAME, ##__VA_ARGS__) result;     \
  } uv_##NAME##__promise_t;                                                    \
  static void __attribute__((unused)) uv_##NAME##__cb _co_tn_arglist(          \
      HorR_TYPE, HorR_NAME, ##__VA_ARGS__) {                                   \
    uv_##NAME##__promise_t *promise =                                          \
        (uv_##NAME##__promise_t *)HorR_NAME->data;                             \
    const typeof(promise->result) result_ =                                    \
        _co_tn_initform(HorR_TYPE, HorR_NAME, ##__VA_ARGS__);                  \
    co_t *waiter = promise->waiter;                                            \
    promise->result = result_;                                                 \
    waiter->fn(waiter, promise, waiter->args, waiter->vars);                   \
  }

_co_define_uv_cb(alloc, uv_handle_t *, handle,
                 size_t, suggested_size,
                 uv_buf_t, buf);
_co_define_uv_cb(read, uv_stream_t *, stream,
                 ssize_t, nread,
                 const uv_buf_t *, buf);
_co_define_uv_cb(write, uv_write_t *, req,
                 int, status);
_co_define_uv_cb(connect, uv_connect_t *, req,
                 int, status);
_co_define_uv_cb(shutdown, uv_shutdown_t *, req,
                 int, status);
_co_define_uv_cb(connection, uv_stream_t *, server,
                 int, status);
_co_define_uv_cb(close, uv_handle_t *, handle);
_co_define_uv_cb(poll, uv_poll_t *, handle,
                 int, status,
                 int, events);
_co_define_uv_cb(timer, uv_timer_t *, handle);
_co_define_uv_cb(async, uv_async_t *, handle);
_co_define_uv_cb(prepare, uv_prepare_t *, handle);
_co_define_uv_cb(check, uv_check_t *, handle);
_co_define_uv_cb(idle, uv_idle_t *, handle);
_co_define_uv_cb(exit, uv_process_t *, process,
                 int64_t, exit_status,
                 int, term_signal);
_co_define_uv_cb(walk, uv_handle_t *, handle,
                 void *, arg);
_co_define_uv_cb(fs, uv_fs_t *, req);
_co_define_uv_cb(work, uv_work_t *, req);
_co_define_uv_cb(after_work, uv_work_t *, req,
                 int, status);
_co_define_uv_cb(getaddrinfo, uv_getaddrinfo_t *, req,
                 int, status,
                 struct addrinfo *, res);
_co_define_uv_cb(getnameinfo, uv_getnameinfo_t *, req,
                 int, status,
                 const char *, hostname,
                 const char *, service);
_co_define_uv_cb(random, uv_random_t *, req,
                 int, status, void *, buf,
                 size_t, buflen);

#define uv_await0(NAME, HANDLE_OR_REQ, ...)                                    \
  do {                                                                         \
    __auto_type _handle_or_req = HANDLE_OR_REQ;                                \
    _co->nested.promise =                                                      \
        realloc(_co->nested.promise, sizeof(uv_##NAME##__promise_t));          \
    ((uv_##NAME##__promise_t *)_co->nested.promise)->waiter = _co;             \
    _handle_or_req->data = _co->nested.promise;                                \
    _co->line = __LINE__;                                                      \
    co_errno = uv_##NAME(HANDLE_OR_REQ, ##__VA_ARGS__, uv_##NAME##__cb);       \
    if (!co_errno)                                                             \
      return;                                                                  \
  case __LINE__:;                                                              \
  } while (false)

#define uv_await(RES_VAR, NAME, ...)                                           \
  uv_await0(NAME, ##__VA_ARGS__);                                              \
  __auto_type RES_VAR = &((uv_##NAME##__promise_t *)_co->nested.promise)->result

#define uv_idle_start__cb uv_idle__cb
#define uv_idle_start__promise_t uv_idle__promise_t
