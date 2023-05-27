/*
This file is part of FlashMQ (https://www.flashmq.org)
Copyright (C) 2021-2023 Wiebe Cazemier

FlashMQ is free software: you can redistribute it and/or modify
it under the terms of The Open Software License 3.0 (OSL-3.0).

See LICENSE for license details.
*/

#ifndef SSLCTXMANAGER_H
#define SSLCTXMANAGER_H

#include <openssl/ssl.h>

class SslCtxManager
{
    SSL_CTX *ssl_ctx = nullptr;
public:
    SslCtxManager();
    ~SslCtxManager();

    SSL_CTX *get() const;
    operator bool() const;
};

#endif // SSLCTXMANAGER_H
