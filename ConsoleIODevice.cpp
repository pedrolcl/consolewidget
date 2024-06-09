// SPDX-License-Identifier: MIT
// Copyright (c) 2020 George Apostolopoulos
// Copyright (c) 2024 Pedro López-Cabanillas <plcl@users.sf.net>

#include "ConsoleIODevice.h"
#include "ConsoleWidget.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QTimer>

ConsoleIODevice::ConsoleIODevice(ConsoleWidget *w, QObject *parent)
    : QIODevice(parent)
    , widget_(w)
    , readpos_(0)
    , writtenSinceLastEmit_(0)
    , readSinceLastEmit_(0)
{
    setCurrentWriteChannel(ConsoleWidget::StandardOutput);
    open(QIODevice::ReadWrite | QIODevice::Unbuffered);
}

ConsoleIODevice::~ConsoleIODevice() {}

qint64 ConsoleIODevice::bytesAvailable() const
{
    return (qint64)(readbuff_.size() - readpos_);
}

bool ConsoleIODevice::waitForReadyRead(int msecs)
{
    if (!widget_->device()->isOpen() || widget_->mode() != ConsoleWidget::Input)
        return false;

    if (bytesAvailable()) return true;


    QEventLoop loop;

    connect(this,SIGNAL(readyRead()),&loop,SLOT(quit()));
    connect(this,SIGNAL(aboutToClose()),&loop,SLOT(quit()));
    if (msecs>0)
        QTimer::singleShot(msecs,&loop,SLOT(quit()));

    readyReadEmmited_ = false;
    loop.exec();
    return readyReadEmmited_;

}

qint64 ConsoleIODevice::readData(char *data, qint64 len)
{
    int b = bytesAvailable();
    if (b) {
        b = qMin((int)len, b);
        memcpy(data, readbuff_.constData() + readpos_, b);
        readpos_ += b;
    }
    return (qint64)b;
}

qint64 ConsoleIODevice::writeData(const char *data, qint64 len)
{
    QByteArray ba(data,(int)len);
    int ch = currentWriteChannel();
    if (ch == ConsoleWidget::StandardError)
        widget_->writeStdErr(ba);
    else
        widget_->writeStdOut(ba);

    writtenSinceLastEmit_ += len;
    if (!signalsBlocked()) {
        emit bytesWritten(writtenSinceLastEmit_);
        writtenSinceLastEmit_ = 0;
    }
    return len;
}

void ConsoleIODevice::consoleWidgetInput(const QString &in)
{
    QByteArray ba = in.toUtf8();
    int sz = ba.size();
    if (bytesAvailable()) {
        if (readpos_) readbuff_ = readbuff_.mid(readpos_);
        readbuff_.append(ba);
    } else {
        readbuff_ = ba;
    }
    readpos_ = 0;

    readSinceLastEmit_ += sz;
    if (!signalsBlocked()) {
        emit readyRead();
        readyReadEmmited_ = true;
        readSinceLastEmit_ = 0;
    }
}

