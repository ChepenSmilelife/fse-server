#include "socketerrorstring.h"

QString socketErrorToString(QAbstractSocket::SocketError error)
{
    QString errorString;
    switch (error) {
    case QAbstractSocket::HostNotFoundError:
        errorString = "The host address was not found.";
        break;
    case QAbstractSocket::SocketAccessError:
        errorString = "The socket operation failed because the application lacked the required privileges.";
        break;
    case QAbstractSocket::SocketResourceError:
        errorString = "The local system ran out of resources (e.g., too many sockets).";
        break;
    case QAbstractSocket::SocketTimeoutError:
        errorString = "The socket operation timed out.";
        break;
    case QAbstractSocket::DatagramTooLargeError:
        errorString = "The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
        break;
    case QAbstractSocket::NetworkError:
        errorString = "An error occurred with the network (e.g., the network cable was accidentally plugged out).";
        break;
    case QAbstractSocket::AddressInUseError:
        errorString = "The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.";
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        errorString = "The address specified to QAbstractSocket::bind() does not belong to the host.";
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        errorString = "The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        errorString = "The socket is using a proxy, and the proxy requires authentication.";
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        errorString = "The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket)";
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        errorString = "Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        errorString = "Could not contact the proxy server because the connection to that server was denied";
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        errorString = "The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established)";
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        errorString = "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
        break;
    case QAbstractSocket::ProxyNotFoundError:
        errorString = "The proxy address set with setProxy() (or the application proxy) was not found.";
        break;
    case QAbstractSocket::ProxyProtocolError:
        errorString = "The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.";
        break;
    case QAbstractSocket::OperationError:
        errorString = "An operation was attempted while the socket was in a state that did not permit it.";
        break;
    case QAbstractSocket::SslInternalError:
        errorString = "The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.";
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        errorString = "Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.";
        break;
    case QAbstractSocket::TemporaryError:
        errorString = "A temporary error occurred (e.g., operation would block and socket is non-blocking).";
        break;
    default:
        errorString = "An unidentified error occurred.";
        break;
    }
    return errorString;
}
