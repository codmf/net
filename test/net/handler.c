#include "net/net.h"

#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/parameterized.h>

#include <stdlib.h>
#include <stdbool.h>

#include "handler.h"

extern NetHandler * gHandler;

// Fix free called from cleanup
void free(void *ptr) {
    cr_free(ptr);
}

// Setup/cleanup tests

Test(handler, setup) {
    gHandler = NULL;
    netSetup();
    cr_assert_not_null(gHandler, "Handler was null even after setup");
    netCleanup();
    cr_expect_null(gHandler, "Handler was not null after setup then cleanup");
}

bool ranCleanup = false;

void customCleanup() {
    ranCleanup = true;
}
#include <stdio.h>
ParameterizedTestParameters(handler, cleanup) {
    const size_t size = 3;
    HandlerParam *params = cr_malloc(sizeof(HandlerParam) * size);

    params[0] = toParam(NULL);
    params[1] = toParam(createEmptyHandler());

    NetHandler * param = createEmptyHandler();
    param->cleanup = &customCleanup;
    fprintf(stdout, "Function: %p\n", param->cleanup);
    params[2] = toParam(param);

    return cr_make_param_array(HandlerParam, params, size, cleanupHandlerParam);
}

ParameterizedTest(HandlerParam * param, handler, cleanup) {
    int pos = 1;
    if (param->handler) {
        param->handler->cleanup = &customCleanup;
    }

    gHandler = param->handler;
    ranCleanup = false;
    cr_log_info("Here %d\n", pos++);
    netCleanup();
    cr_log_info("Here %d\n", pos++);
    cr_assert_null(gHandler, "Handler was not null after cleanup");
    cr_log_info("Here %d\n", pos++);

    cr_log_info("Here %d\n", pos++);
    if (param->handler && param->handler->cleanup) {
        cr_log_info("Here %d\n", pos++);
        cr_assert(ranCleanup, "Custom cleanup function did not change global variable");
    }
}

// Set handler test

ParameterizedTestParameters(handler, set) {
    const size_t size = 2;
    HandlerParam *params = cr_malloc(sizeof(HandlerParam) * size);

    params[0] = toParam(NULL);
    params[1] = toParam(createEmptyHandler());

    return cr_make_param_array(HandlerParam, params, size, cleanupHandlerParam);
}

ParameterizedTest(HandlerParam * param, handler, set) {
    gHandler = NULL;
    netSetHandler(param->handler);
    cr_assert_eq(gHandler, param->handler, "Actual handler was not the expected one");
}

// Get handler

ParameterizedTestParameters(handler, get) {
    const size_t size = 2;
    HandlerParam *params = cr_malloc(sizeof(HandlerParam) * size);

    params[0] = toParam(NULL);
    params[1] = toParam(createEmptyHandler());

    return cr_make_param_array(HandlerParam, params, size, cleanupHandlerParam);
}

ParameterizedTest(HandlerParam * param, handler, get) {
    gHandler = param->handler;
    cr_assert_eq(netGetHandler(), param->handler, "Actual handler was not the one that was expected");
}
