#ifndef QCACHEDPEER_H
#define QCACHEDPEER_H

#include <QTimer>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>


QT_BEGIN_NAMESPACE

class QCachedPeer : public QObject
{
    Q_OBJECT
public:
    QCachedPeer();
    virtual ~QCachedPeer();
    X509* getX509() { return m_x509; }
    void setX509(X509* x509);
    void refresh();

public slots:
    void clear();

private:
    X509* m_x509;
    QTimer* m_timer;
};

QT_END_NAMESPACE
#endif
