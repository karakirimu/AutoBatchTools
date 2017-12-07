#include "consoleview.h"

ConsoleView::ConsoleView(QWidget *parent)
    : ConsoleBase(parent)
{
}

ConsoleView::~ConsoleView()
{
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
