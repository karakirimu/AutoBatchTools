#include "consoleview.h"

ConsoleView::ConsoleView(QWidget *parent)
    : QTextEdit(parent)
{
    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    //init menu context
    setPopupAction();
}

ConsoleView::~ConsoleView()
{
    delete contextMenu;
}

void ConsoleView::setRunTaskSignalBinder(RunTaskSignalBinder *rbinder)
{
    this->binder = rbinder;

    connect(binder, &RunTaskSignalBinder::processStarted, this, &ConsoleView::started);
    connect(binder, &RunTaskSignalBinder::processPaused, this, &ConsoleView::pause);
    connect(binder, &RunTaskSignalBinder::processStopped, this, &ConsoleView::stop);
    connect(binder, &RunTaskSignalBinder::processEnd, this, &ConsoleView::end);
    connect(binder, &RunTaskSignalBinder::processMessage, this, &ConsoleView::updateText);
    connect(binder, &RunTaskSignalBinder::processErrorText, this, &ConsoleView::updateExternalError);

}

void ConsoleView::started(int runtype)
{
    setTextColor(QColor(Qt::darkGray));
    switch (runtype) {
    case Executor::DEFAULT:
        append(tr("*****PROCESS STARTED -  TYPE DEFAULT *****\r\n"));
        break;

    case Executor::SCHEDULER:
        append(tr("*****PROCESS STARTED - TYPE SCHEDULER*****\r\n"));
        break;

    default:
        break;
    }
    this->textCursor().movePosition(QTextCursor::End);
//    isReady = false;
}

void ConsoleView::pause()
{
    setTextColor(QColor(Qt::darkGray));
    append(tr("***** PROCESS PAUSED *****\r\n"));
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleView::stop()
{
    setTextColor(QColor(Qt::darkGray));
    append(tr("***** PROCESS STOPPED *****\r\n"));
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleView::end(int runtype)
{
    setTextColor(QColor(Qt::darkGray));
    switch (runtype) {
    case Executor::DEFAULT:
        append(tr("*****PROCESS COMPLETED - TYPE DEFAULT*****\r\n"));
        break;

    case Executor::SCHEDULER:
        append(tr("*****PROCESS COMPLETED - TYPE SCHEDULER*****\r\n"));
        break;

    default:
        break;
    }
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleView::updateText(QString data, int type)
{
    switch (type) {
    case Executor::INFO:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::NORMAL:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::SEARCH:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::SCRIPT:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::OTHER:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::ERROR:
        setTextColor(QColor(Qt::red));
        append(data);
        break;
    case Executor::TEMP:
    case Executor::LOCAL:
    default:
        break;
    }

    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleView::updateExternalError(QString str)
{
    setTextColor(QColor(Qt::red));
    append(str);
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleView::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

void ConsoleView::setPopupAction()
{
    contextMenu = new QMenu(this);
    contextMenu->setStyleSheet("border:1px;");
    m_selectall = contextMenu->addAction(tr("Select All"));
    m_copy = contextMenu->addAction(QIcon(":/icons/Copy.png"), tr("Copy"));
    contextMenu->addSeparator();
    m_clear = contextMenu->addAction(tr("Clear all text"));

    //connect signals
    connect(m_selectall, &QAction::triggered, this, &QTextEdit::selectAll);
    connect(m_copy, &QAction::triggered, this, &QTextEdit::copy);
    connect(m_clear, &QAction::triggered, this, &QTextEdit::clear);
}
