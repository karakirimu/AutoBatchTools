#include "extraplugininterface.h"

ExtraPluginInterface::ExtraPluginInterface(){}

void ExtraPluginInterface::setLocalValue(QHash<QString, QString> *local){ this->local = local; }

void ExtraPluginInterface::setGlobalValue(QHash<QString, QString> global){this->global= global;}

void ExtraPluginInterface::setInputFileData(QStringList file){this->file  = file;  }

QString ExtraPluginInterface::functionErrorMessage(){ return errorMessage; }

QString ExtraPluginInterface::functionSuccessMessage(){ return successMessage; }

void ExtraPluginInterface::setErrorMessage(const QString message){ errorMessage = message; }

void ExtraPluginInterface::setSuccessMessage(const QString message){ successMessage = message; }
