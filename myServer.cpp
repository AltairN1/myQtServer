#include "myServer.h"

myServer::myServer(int nPort, QWidget* pwgt) : QWidget(pwgt),
                                               m_nNextBlockSize(0){
    m_ptcpServer = new QTcpServer(this);
    if(!m_ptcpServer->listen(QHostAddress::Any, nPort)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"+
                              m_ptcpServer->errorString());
        m_ptcpServer->close();
        return;
    }
    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);
}

void myServer::slotNewConnection(){
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(deleteLater()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
    sendToClient(pClientSocket, "Server Response: Connected!");
}

void myServer::slotReadClient() {
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in (pClientSocket);
    in.setVersion(QDataStream::Qt_6_1);
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }

            in >> m_nNextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize){
            break;
        }
        QTime time;
        QString str;
        in >> time >> str;
        m_ptxt->append(str);
        m_nNextBlockSize = 0;
        sendToClient(pClientSocket, "Server Response: Received \"" + str + "\"");
        SaveAs();
    }
}

void myServer::sendToClient(QTcpSocket *pSocket, const QString &str){
    QByteArray arrBlock;
    QDataStream out (&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_1);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size()-sizeof(quint16));
    pSocket->write(arrBlock);
}

void myServer::SaveAs() {
    QString str = QFileDialog::getSaveFileName(0, m_strFileName);
    m_strFileName = str;
    Save();
}

void myServer::Save(){
    QFile file(m_strFileName);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream(&file) << m_ptxt->toPlainText();
        file.close();
        emit changeWindowTitle(m_strFileName);
        m_ptxt->clear();
    }
}
