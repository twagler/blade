#ifndef MAPPINGSERVICE_H
#define MAPPINGSERVICE_H

#include <QObject>

class MappingService : public QObject
{
    Q_OBJECT
public:
    explicit MappingService(QObject *parent = 0);

signals:

public slots:
};

#endif // MAPPINGSERVICE_H