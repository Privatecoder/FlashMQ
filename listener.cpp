#include "listener.h"

#include "utils.h"
#include "exceptions.h"

void Listener::isValid()
{
    if (isSsl())
    {
        if (port == 0)
        {
            if (websocket)
                port = 4443;
            else
                port = 8883;
        }

        testSsl(sslFullchain, sslPrivkey);
    }
    else
    {
        if (port == 0)
        {
            if (websocket)
                port = 8080;
            else
                port = 1883;
        }
    }

    if (port <= 0 || port > 65534)
    {
        throw ConfigFileException(formatString("Port nr %d is not valid", port));
    }
}

bool Listener::isSsl() const
{
    return (!sslFullchain.empty() || !sslPrivkey.empty());
}

std::string Listener::getProtocolName() const
{
    if (isSsl())
    {
        if (websocket)
            return "SSL websocket";
        else
            return "SSL TCP";
    }
    else
    {
        if (websocket)
            return "non-SSL websocket";
        else
            return "non-SSL TCP";
    }

    return "whoops";
}

void Listener::loadCertAndKeyFromConfig()
{
    if (!isSsl())
        return;

    if (!sslctx)
    {
        sslctx.reset(new SslCtxManager());
        SSL_CTX_set_options(sslctx->get(), SSL_OP_NO_SSLv3); // TODO: config option
        SSL_CTX_set_options(sslctx->get(), SSL_OP_NO_TLSv1); // TODO: config option
    }

    if (SSL_CTX_use_certificate_file(sslctx->get(), sslFullchain.c_str(), SSL_FILETYPE_PEM) != 1)
        throw std::runtime_error("Loading cert failed. This was after test loading the certificate, so is very unexpected.");
    if (SSL_CTX_use_PrivateKey_file(sslctx->get(), sslPrivkey.c_str(), SSL_FILETYPE_PEM) != 1)
        throw std::runtime_error("Loading key failed. This was after test loading the certificate, so is very unexpected.");
}

std::string Listener::getBindAddress(ListenerProtocol p)
{
    if (p == ListenerProtocol::IPv4)
    {
        if (inet4BindAddress.empty())
            return "0.0.0.0";
        return inet4BindAddress;
    }
    if (p == ListenerProtocol::IPv6)
    {
        if (inet6BindAddress.empty())
            return "::";
        return inet6BindAddress;
    }
    return "";
}
