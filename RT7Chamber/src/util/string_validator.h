#ifndef STRINGVALIDATOR_H
#define STRINGVALIDATOR_H

#include <QString>

class StringValidator
{
    StringValidator() = delete;
public:
    static QString ValidateIPByte(const QString& strByteIP);
    static QString ValidatePort(const QString& strPort);
};

#endif // STRINGVALIDATOR_H
