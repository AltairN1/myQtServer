#pragma once
#include <QtWidgets>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QFileDialog>
class QTcpServer;
class QTextEdit;
class QTcpSocket;

class myServer : public QWidget {
    Q_OBJECT
    QTcpServer* m_ptcpServer;
    QTextEdit* m_ptxt;
    quint16 m_nNextBlockSize;
    QString m_strFileName;
    void sendToClient(QTcpSocket* pSocket, const QString& str);
public:
    myServer(int nPort, QWidget* pwgt = 0);
signals:
    void changeWindowTitle(const QString&);
public slots:
    virtual void slotNewConnection();
    void slotReadClient();
    void Save();
    void SaveAs();
};
