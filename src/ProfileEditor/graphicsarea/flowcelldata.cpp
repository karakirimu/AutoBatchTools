#include "flowcelldata.h"

FlowCellData::FlowCellData()
{
    //set default color
    framecolor = "background-color: transparent;";
    typecolor = "background-color: white;";
    content = "";
    typepixmap = QPixmap(24, 24);
    cellheight = 150;
}

FlowCellData::~FlowCellData()
{

}

QString FlowCellData::getTypecolor() const
{
    return typecolor;
}

void FlowCellData::setTypecolor(const QString &csstext)
{
    typecolor = csstext;
}

QString FlowCellData::getFramecolor() const
{
    return framecolor;
}

void FlowCellData::setFramecolor(const QString &csstext)
{
    framecolor = csstext;
}

QString FlowCellData::getContent() const
{
    return content;
}

void FlowCellData::setContent(const QString &htmlstr)
{
    content = htmlstr;
}

QPixmap FlowCellData::getTypepixmap() const
{
    return typepixmap;
}

void FlowCellData::setTypepixmap(const QPixmap &value)
{
    typepixmap = value;
}

QString FlowCellData::getType() const
{
    return type;
}

void FlowCellData::setType(const QString &value)
{
    type = value;
}

int FlowCellData::height() const
{
    return cellheight;
}

void FlowCellData::setheight(int value)
{
    cellheight = value;
}
