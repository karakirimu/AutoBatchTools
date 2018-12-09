#include "consolebase.h"


ConsoleBase::ConsoleBase(QWidget *parent)
    : QTextEdit(parent)
{
    //install custom context
    setContextMenuPolicy(Qt::CustomContextMenu);
    installEventFilter(this);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    //init menu context
    setPopupAction();

    connect(this, &ConsoleBase::cursorPositionChanged, this, &ConsoleBase::moveCursorToEnd);

    //hide vertical scrollbar
    this->verticalScrollBar()->hide();
    this->horizontalScrollBar()->hide();

}

ConsoleBase::~ConsoleBase()
{
    delete contextMenu;
}

void ConsoleBase::started(int runtype)
{
    setTextColor(QColor(Qt::darkGray));
    switch (runtype) {
    case Executor::DEFAULT:
        append(tr("***** PROCESS STARTED *****\r\n"));
        break;

    case Executor::SCHEDULER:
        append(tr("***** PROCESS STARTED - SCHEDULER *****\r\n"));
        break;

    default:
        break;
    }

    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleBase::pause()
{
    setTextColor(QColor(Qt::darkGray));
    append(tr("***** PROCESS PAUSED *****\r\n"));
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleBase::stop()
{
    setTextColor(QColor(Qt::darkGray));
    append(tr("***** PROCESS STOPPED *****\r\n"));
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleBase::end(int runtype)
{
    setTextColor(QColor(Qt::darkGray));
    switch (runtype) {
    case Executor::DEFAULT:
        append(tr("***** PROCESS COMPLETED *****\r\n"));
        break;

    case Executor::SCHEDULER:
        append(tr("***** PROCESS COMPLETED - SCHEDULER *****\r\n"));
        break;

    default:
        break;
    }
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleBase::updateText(QString data, int type)
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
    case Executor::PLUGINS:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::OTHER:
        setTextColor(QColor(Qt::darkGray));
        append(data);
        break;
    case Executor::WARNING:
        setTextColor(QColor(Qt::yellow));
        append(data);
        break;
    case Executor::ERROR:
        setTextColor(QColor(Qt::red));
        append(data);
        break;
    case Executor::INPUT:
        setTextColor(QColor(Qt::darkYellow));
        append(data);
        break;
    case Executor::TEMP:
    case Executor::LOCAL:
    default:
        break;
    }

    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleBase::updateExternalError(QString str)
{
    setTextColor(QColor(Qt::red));
    append(str);
    this->textCursor().movePosition(QTextCursor::End);
}

void ConsoleBase::onCustomContextMenu(const QPoint &point)
{
    contextMenu->popup(mapToGlobal(point));
}

void ConsoleBase::setPopupAction()
{
    contextMenu = new QMenu(this);
    contextMenu->setStyleSheet("border:1px;");
    m_selectall = contextMenu->addAction(tr("Select All"));
    m_copy = contextMenu->addAction(QIcon(":/default_icons/copy.png"), tr("Copy"));
    contextMenu->addSeparator();
    m_clear = contextMenu->addAction(tr("Clear all text"));

    //connect signals
    connect(m_selectall, &QAction::triggered, this, &QTextEdit::selectAll);
    connect(m_copy, &QAction::triggered, this, &QTextEdit::copy);
    connect(m_clear, &QAction::triggered, this, &QTextEdit::clear);
}

void ConsoleBase::moveCursorToEnd()
{
    this->textCursor().movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
}

bool ConsoleBase::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Leave){
        this->horizontalScrollBar()->hide();
        this->verticalScrollBar()->hide();
    }

    if(event->type() == QEvent::ToolTip){
        if(this->mapFromGlobal(QCursor::pos()).y() > (this->height() - RANGE)){
            this->horizontalScrollBar()->show();
        }
        if(this->mapFromGlobal(QCursor::pos()).x() > (this->width() - RANGE)){
            this->verticalScrollBar()->show();
        }
    }

    // standard event processing
    return QObject::eventFilter(obj, event);
}
