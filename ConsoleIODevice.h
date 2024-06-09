// SPDX-License-Identifier: MIT
// Copyright (c) 2020 George Apostolopoulos
// Copyright (c) 2024 Pedro López-Cabanillas <plcl@users.sf.net>

#ifndef CONSOLEIODEVICE_H
#define CONSOLEIODEVICE_H

#include <QIODevice>
#include <QByteArray>

class ConsoleWidget;

class ConsoleIODevice : public QIODevice
{
    Q_OBJECT

public:
    explicit ConsoleIODevice(ConsoleWidget *w, QObject *parent = nullptr);
    ~ConsoleIODevice();
    qint64 bytesAvailable() const override;
    bool waitForReadyRead(int msecs) override;
    ConsoleWidget *widget() const { return widget_; }

protected:

    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:
    friend class ConsoleWidget;
    ConsoleWidget *widget_;
    QByteArray readbuff_;
    int readpos_;
    qint64 writtenSinceLastEmit_, readSinceLastEmit_;
    bool readyReadEmmited_;
    void consoleWidgetInput(const QString& in);
};

#endif
