#include "nativesdkmanager.h"
#include <QMutexLocker>
#include <QDebug>

NativeSdkManager* NativeSdkManager::m_pNativeSdkManager = NULL;
NativeSdkManager::NativeSdkManager(){

}
NativeSdkManager::~NativeSdkManager(){
    QMap<QString,NativeSdkHandlerBase*> handlers = m_NativeSdkFactory.getAllHandlers();
    QMapIterator<QString,NativeSdkHandlerBase*> i(handlers);
    while (i.hasNext()) {
        i.next();
        NativeSdkHandlerBase * handler = i.value();
        if(handler){
            disconnect(handler,SIGNAL(sucess(long,QVariant)),this,SIGNAL(sucess(long,QVariant)));
            disconnect(handler,SIGNAL(failed(long,long,QString)),this,SIGNAL(failed(long,long,QString)));
            disconnect(handler,SIGNAL(progress(long,int,int,int)),this,SIGNAL(progress(long,int,int,int)));
            delete handler;
            handler = NULL;
        }
    }
    if(m_pNativeSdkManager){
        delete m_pNativeSdkManager;
        m_pNativeSdkManager = NULL;
    }


}
NativeSdkManager * NativeSdkManager::getInstance(){
    static QMutex mutex;
    if(m_pNativeSdkManager == NULL){
        QMutexLocker locker(&mutex);
        if(m_pNativeSdkManager == NULL)
            m_pNativeSdkManager = new NativeSdkManager;
    }
    return m_pNativeSdkManager;
}
void NativeSdkManager::request(QString className,QString callBackID,QString actionName,QVariantMap params){
    NativeSdkHandlerBase * handler = m_NativeSdkFactory.getHandler(className);
    if(handler){
        if(!m_NativeSdkFactory.IsInitConnect(className))
            initHandlerConnect(className);
        handler->request(callBackID,actionName,params);

    }else{
        qDebug()<< "模块:"<<className<<"不存在";
    }

}
void NativeSdkManager::submit(QString typeID,QString callBackID,QString actionName,QVariant dataRowList, QVariant attachementes){

    Q_UNUSED(typeID)
    Q_UNUSED(callBackID)
    Q_UNUSED(actionName)
    Q_UNUSED(dataRowList)
    Q_UNUSED(attachementes)
}
QObject * NativeSdkManager::getUiSource(QString typeID,QString actionName){
    NativeSdkHandlerBase * handler = m_NativeSdkFactory.getHandler(typeID);
    QObject * item = NULL;
    if(handler){
        if(!m_NativeSdkFactory.IsInitConnect(typeID))
            initHandlerConnect(typeID);
        item =  handler->getUiSource(actionName);
    }
    return item;
}
void NativeSdkManager::initHandlerConnect(QString typeID){
    NativeSdkHandlerBase * handler = m_NativeSdkFactory.getAllHandlers().value(typeID);
    if(handler){
        connect(handler,SIGNAL(sucess(long,QVariant)),this,SIGNAL(sucess(long,QVariant)));
        connect(handler,SIGNAL(failed(long,long,QString)),this,SIGNAL(failed(long,long,QString)));
        connect(handler,SIGNAL(progress(long,int,int,int)),this,SIGNAL(progress(long,int,int,int)));
    }
}
void NativeSdkManager::loadQml(QString typeID,QString parentPageName, QString parentName, QString type){
    NativeSdkHandlerBase * handler = m_NativeSdkFactory.getHandler(typeID);
    if(handler){
        if(!m_NativeSdkFactory.IsInitConnect(typeID))
            initHandlerConnect(typeID);
        handler->loadQml(parentPageName,parentName,type);

    }
}

