#include "string_validator.h"

QString StringValidator::ValidateIPByte(const QString& strByteIP)
{
    try {
        int resInt = std::stoi(strByteIP.toStdString());
        if(resInt < 0 || resInt > 255)
        {
            return QString("0");
        }
        else
        {
            return QString::fromStdString(std::to_string(resInt));
        }
    } catch (...)
    {
        return QString("0");
    }
}

QString StringValidator::ValidatePort(const QString& strPort)
{
    try {
        int resInt = strPort.toInt();
        if(resInt < 0 || resInt > 65535)
        {
            return QString("0");
        }
        else
        {
            return QString::fromStdString(std::to_string(resInt));
        }
    } catch (...)
    {
        return QString("0");
    }

}

