# ConsoleWidget

This component was developed by George Apostolopoulos in 2020 as part of [**QDaq** - Qt-based Data Acquisition](https://gitlab.com/qdaq/qdaq) 
and forked by Pedro López-Cabanillas in 2024 from [this GitHub repository](https://github.com/gapost/qconsolewidget).

A lightweight Qt console widget based on QPlainTextEdit providing 
also a QIODevice interface.

It can be used as an interactive scripting terminal or a log window.
Features include standard console-like editing, command history, 
formatted input, output & error streams.

TODO: syntax highlighting

## Usage

Instantiate the widget and set it to input mode. Connect a QObject slot
to the consoleCommand signal to receive the user input.

```c++
ConsoleWidget w;
w.writeStdOut("enter command> ");
w.setMode(ConsoleWidget::Input);
QObject::connect(&w,SIGNAL(consoleCommand(QString)),MyQObject,SLOT(evalCommand(QString)))
...
MyQObjet::evalCommand(const QString& code)
{
    ...
}
```

Alternatively you can use a QTextStream to interact with ConsoleWidget:

```c++
ConsoleWidget w;
w.device()->open(QIODevice::ReadWrite); // open the console's QIODevice
QTextStream stream(w.device());
stream << "Hello World!" << endl; // output goes to the widget
```

The stream can also be used for input

```c++
stream << "Enter an integer n = " << flush;
int n;
stream.device()->waitForReadyRead();
stream >> n;
```
The call to ```waitForReadyRead()``` enters a local loop waiting for
the user to enter a command and hit return. 

## License

SPDX-License-Identifier: MIT
Copyright (c) 2020 George Apostolopoulos
Copyright (c) 2024 Pedro López-Cabanillas <plcl@users.sf.net>
