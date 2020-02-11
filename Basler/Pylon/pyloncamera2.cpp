#include "pyloncamera.h"

using namespace Pylon;
using namespace GenApi;

static QPair<QString, int> concurentLoad(QString FileName,
                     GENAPI_NAMESPACE::INodeMap* pNodeMap, int Numb)
{
    QPair<QString, int> pair;
    pair.second=Numb;
    try
    {
        CFeaturePersistence::Load(FileName.toLocal8Bit().constData(),pNodeMap,true);
        pair.first="Succ";
        return pair;
    }
    catch (const GenericException &e)
    {
        pair.first=e.GetDescription();
        return pair;
    }
}

static QPair<QString, int> concurentSave(QString FileName,
                     GENAPI_NAMESPACE::INodeMap* pNodeMap, int Numb)
{
    QPair<QString, int> pair;
    pair.second=Numb;
    try
    {
        CFeaturePersistence::Save(FileName.toLocal8Bit().constData(),pNodeMap);
        pair.first="Succ";
        return pair;
    }
    catch (const GenericException &e)
    {
        pair.first=e.GetDescription();
        return pair;
    }
}

void PylonCamera::loadFeaturesFinished()
{
   QFutureWatcher<QPair<QString, int>>* watcher = reinterpret_cast<QFutureWatcher<QPair<QString, int>>*>(sender());
   QPair<QString, int> pair=watcher->result();
   if (pair.first=="Succ")
   {
       emit Message(LoggerMsg::Info,"BSL: SET: Настройки камеры загружены успешно");
       videoServer->CurrentMode=pair.second;
   } else
   {
       emit Message(LoggerMsg::Warning,QString("BSL: SET: Ошибка загрузки настроек  %1: папка %2").arg(pair.first,QString(QDir::currentPath()+QString::number(pair.second))));
       videoServer->CurrentMode=MODE_UNDEFINED;
   }
}

void PylonCamera::saveFeaturesFinished()
{
    QFutureWatcher<QPair<QString, int>>* watcher = reinterpret_cast<QFutureWatcher<QPair<QString, int>>*>(sender());
    QPair<QString, int> pair=watcher->result();
    if (pair.first=="Succ")
    {
        emit Message(LoggerMsg::Info,"BSL: SET: Настройки камеры сохранены успешно");
        videoServer->CurrentMode=pair.second;
    } else
    {
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Ошибка сохранения настроек  %1: папка %2").arg(pair.first,QString(QDir::currentPath()+QString::number(pair.second))));
        videoServer->CurrentMode=MODE_UNDEFINED;
    }
}

void PylonCamera::loadFeatures(int numb)
{
    QFuture<QPair<QString, int>> future = QtConcurrent::run(concurentLoad,QString().sprintf("%s/%d.pfs",QDir::currentPath().toLocal8Bit().constData(),numb),&camera->GetNodeMap(),numb);
    loadWatcher.setFuture(future);
}

void PylonCamera::saveFeatures(int numb)
{
    QFuture<QPair<QString, int>> future = QtConcurrent::run(concurentSave,QString().sprintf("%s/%d.pfs",QDir::currentPath().toLocal8Bit().constData(),numb),&camera->GetNodeMap(),numb);
    saveWatcher.setFuture(future);
}

QList<int> PylonCamera::getIntPtrFeatures(int numb)
{
    QList<int> List;
    if (!isCameraOpening) return List;
    for (int i=0;i<5;i++,List.append(0));
    GenICam::gcstring  name=nameSpace->getName(numb,PylonFeaturesEnum::DInt);
    if (name)
    if (!getIntPtrFeatures(name, List))
    {
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Ошибка инициализации параметра %1").arg(nameSpace->getNameRU(numb,PylonFeaturesEnum::DInt)));
        return List;
    }
    List[0]=1;
    return List;
}

bool PylonCamera::getIntPtrFeatures(GenICam::gcstring &name,QList<int>&L)
{
    INodeMap* mNodeMap=&(camera->GetNodeMap());
    const CIntegerPtr IntPtr=mNodeMap->GetNode(name);
    if (!IntPtr.IsValid()) return false;
    L[1]=static_cast<int>(IntPtr->GetMin());
    L[2]=static_cast<int>(IntPtr->GetMax());
    L[3]=static_cast<int>(IntPtr->GetValue());
    L[4]=static_cast<int>(IntPtr->GetInc());
    return true;
}

bool PylonCamera::setIntPtrFeatures(int numb,int value)
{
    GenICam::gcstring  name=nameSpace->getName(numb,PylonFeaturesEnum::DInt);
    if (!name) return false;
    try{
        if (setIntPtrFeatures(name,value))
        {
            emit Message(LoggerMsg::Info,QString("BSL: SET: Параметр %1 = %2.").arg(QString(nameSpace->getNameRU(numb,PylonFeaturesEnum::DInt)),QString::number(value)));
            return true;
        }
    }
    catch (GenICam_3_1_Basler_pylon_v5_1::RuntimeException ex)
    {
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Параметр %1 != %2. %3").arg(QString(nameSpace->getNameRU(numb,PylonFeaturesEnum::DInt)),QString::number(value),QString(ex.GetDescription())));
    }
    return false;
}

bool PylonCamera::setIntPtrFeatures(GenICam::gcstring &name,int &value)
{
    if (!isCameraOpening) return false;
    try {
        INodeMap* mNodeMap=&(camera->GetNodeMap());
        const CIntegerPtr IntPtr=mNodeMap->GetNode(name);
        if (!IntPtr.IsValid()) return false;
        if (IsWritable(IntPtr))
        {
            IntPtr->SetValue(value);
        }
        const CIntegerPtr IntPtr2=mNodeMap->GetNode(name);
        if (static_cast<int>(IntPtr2->GetValue())!=value) return false;
        return true;
    } catch (OutOfRangeException ex)
    {
        return false;
    }
}

QList<double> PylonCamera::getFloatFeatures(int numb)
{
    QList<double> List;
    if (!isCameraOpening) return List;
    for (int i=0;i<4;i++,List.append(0.0));
    GenICam::gcstring  name=nameSpace->getName(numb,PylonFeaturesEnum::DFloat);
    if (name)
    if (!getFloatFeatures(name, List))
    {
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Ошибка инициализации параметра %1").arg(nameSpace->getNameRU(numb,PylonFeaturesEnum::DFloat)));
        return List;
    }
    List[0]=1.0;
    return List;
}

bool PylonCamera::getFloatFeatures(GenICam::gcstring  &name, QList<double> &L)
{
    INodeMap* mNodeMap=&(camera->GetNodeMap());
    const CFloatPtr Ptr=mNodeMap->GetNode(name);
    if (!Ptr.IsValid()) return false;
    L[1]=static_cast<double>(Ptr->GetMin());
    L[2]=static_cast<double>(Ptr->GetMax());
    L[3]=static_cast<double>(Ptr->GetValue());
    return true;
}

bool PylonCamera::setFloatFeatures(int numb,double value)
{
    GenICam::gcstring  name=nameSpace->getName(numb,PylonFeaturesEnum::DFloat);
    if (!name) return false;
    try{
        if (setFloatFeatures(name,value))
        {
            emit Message(LoggerMsg::Info,QString("BSL: SET: Параметр %1 = %2.").arg(QString(nameSpace->getNameRU(numb,PylonFeaturesEnum::DFloat)),QString::number(value)));
            return true;
        }
    }
    catch (GenICam_3_1_Basler_pylon_v5_1::RuntimeException ex)
    {
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Параметр %1 != %2. %3").arg(QString(nameSpace->getNameRU(numb,PylonFeaturesEnum::DFloat)),QString::number(value),QString(ex.GetDescription())));
    }
    return false;
}

bool PylonCamera::setFloatFeatures(GenICam::gcstring  &name,double &value)
{
    if (!isCameraOpening) return false;
    try {
        INodeMap* mNodeMap=&(camera->GetNodeMap());
        const CFloatPtr Ptr=mNodeMap->GetNode(name);
        if (!Ptr.IsValid()) return false;
        if (IsWritable(Ptr))
        {
            Ptr->SetValue(static_cast<double>(value));
        } else  return false;
        const CFloatPtr Ptr2=mNodeMap->GetNode(name);
        if (!Ptr2.IsValid()) return false;
        //у камеры некоторые параметры имеют шаг установки. В документации шаг не обозначен. От балды
        if (fabs(static_cast<double>(Ptr2->GetValue())-value)>0.01)//std::numeric_limits<double>::epsilon())
        {
            qWarning()<<Ptr2->GetValue()<<" "<<value;
            return false;
        }
        return true;
    } catch (std::exception ex)
    {
        return false;
    }
}

bool PylonCamera::setEnumFeatures(int numb, int value)
{
    QString valToStr;
    GenICam::gcstring  name=nameSpace->getName(numb,PylonFeaturesEnum::DEnum);
    if (!name) return false;
    if (setEnumFeatures(name,value,valToStr))
    //проверяем
    if (getEnumCurrentValueToInt(numb)==value)
    {
        emit Message(LoggerMsg::Info,QString("BSL: SET: Параметр %1 = %2.").arg(QString(nameSpace->getNameRU(numb,PylonFeaturesEnum::DEnum)),valToStr));
        return true;
    }
    emit Message(LoggerMsg::Warning,QString("BSL: SET: Параметр %1 != %2.").arg(QString(nameSpace->getNameRU(numb,PylonFeaturesEnum::DEnum)),valToStr));
    return false;
}

bool PylonCamera::setEnumFeatures(GenICam::gcstring  &name, int &value, QString & valToStr)
{
    if (!isCameraOpening) return false;
    //т.к. у нас русский интерфейс, придется тут расшифровывать значение
    QStringList SL;
    getEnumFeatures(name,SL);
    if (SL.size()<=value) return false;
    valToStr=SL[value];
    //теперь устанавливаем значение
    INodeMap* mNodeMap=&(camera->GetNodeMap());
    const CEnumerationPtr EnumPtr=mNodeMap->GetNode(name);
    if (!EnumPtr.IsValid()) return  false;
    if (!IsWritable(EnumPtr)) return false;
    EnumPtr->FromString(valToStr.toUtf8().constData());
    return true;
}

int PylonCamera::getEnumCurrentValueToInt(int numb)
{
    if (!camera) return 0;
    int Result=-1;
    GenICam::gcstring  name=nameSpace->getName(numb,PylonFeaturesEnum::DEnum);
    if (!name) return Result;
    INodeMap* mNodeMap=&(camera->GetNodeMap());
    const CEnumerationPtr EnumPtr=mNodeMap->GetNode(name);
    if (!EnumPtr.IsValid()) return  Result;
    Result=static_cast<int>(EnumPtr->GetIntValue());
    return Result;
}

//file:///opt/pylon5/share/doc/C++/nameSpace_basler___gig_e_camera.html#a61ffc0f9ac98b7eb04368aa4c848e485a588db700b2dad21f2e9c383350095b3b
//использовалась для предоставления модели для combobox. Из-за англ языка не исп.
bool PylonCamera::getEnumFeatures(GenICam::gcstring  &name,QStringList &Result)
{
    if (!camera) return false;
    GenICam::gcstring_vector temp;
    INodeMap* mNodeMap=&(camera->GetNodeMap());
    const CEnumerationPtr EnumPtr=mNodeMap->GetNode(name);
    if (!EnumPtr.IsValid()) return  false;
    EnumPtr->GetSymbolics(temp);
    for (auto n:temp)
    {
        Result.append(n.c_str());
    }
    return true;
}

bool PylonCamera::getGammaEnable()
{
    if (!camera) return false;
    return camera->GammaEnable.GetValue(false);
}

void PylonCamera::setGammaEnable(bool _GammaEnable)
{
    if (!camera) return;
    camera->GammaEnable.SetValue(_GammaEnable);
    QString gamma;
    _GammaEnable? gamma="вкл" : gamma="выкл";
    if (getGammaEnable()==_GammaEnable)
        emit Message(LoggerMsg::Info,QString("BSL: SET: Гамма = "+gamma ));
    else
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Гамма != "+gamma ));
}

double PylonCamera::getTemperatureABS()
{
    if (!camera) return 0.;
    //сообщение о разрыве соединения с камерой приходит поздно. Так что
    try{
        return camera->TemperatureAbs.GetValue(false);
    }
    catch (GenICam_3_1_Basler_pylon_v5_1::RuntimeException ex)
    {
        qWarning()<<"olikke good error"<<ex.what();
        return 0.;
    }

}

bool PylonCamera::getRateControl()
{
    if (!camera) return false;
    return camera->AcquisitionFrameRateEnable.GetValue(false);
}

void PylonCamera::setRateControl(bool _RateControl)
{
    if (!camera) return;
    camera->AcquisitionFrameRateEnable.SetValue(_RateControl);
    QString rate;
    _RateControl? rate="вкл" : rate="выкл";
    if (getRateControl()==_RateControl)
        emit Message(LoggerMsg::Info,QString("BSL: SET: Задать частоту кадров = "+rate ));
    else
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Задать частоту кадров != "+rate ));
}

bool PylonCamera::getXCenter()
{
    if (!camera) return false;
    return camera->CenterX.GetValue(false);
}

void  PylonCamera::setXCenter(bool _XCenter)
{
    if (!camera) return;
    camera->CenterX.SetValue(_XCenter);
    QString center;
    _XCenter? center="вкл" : center="выкл";
    if (getXCenter()==_XCenter)
        emit Message(LoggerMsg::Info,QString("BSL: SET: Центрирование кадра по Х = "+center ));
    else
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Центрирование кадра по Х != "+center ));
}

bool PylonCamera::getYCenter()
{
    if (!camera) return false;
    return camera->CenterY.GetValue(false);
}

void  PylonCamera::setYCenter(bool _YCenter)
{
    if (!camera) return;
    camera->CenterY.SetValue(_YCenter);
    QString center;
    _YCenter? center="вкл" : center="выкл";
    if (getYCenter()==_YCenter)
        emit Message(LoggerMsg::Info,QString("BSL: SET: Центрирование кадра по Y = "+center ));
    else
        emit Message(LoggerMsg::Warning,QString("BSL: SET: Центрирование кадра по Y != "+center ));
}

bool PylonCamera::getStringFeatures(GenICam::gcstring &name, QString & val)
{
    if (!camera) return false;
    INodeMap* mNodeMap=&(camera->GetNodeMap());
    const CStringPtr Ptr=mNodeMap->GetNode(name);
    if (!Ptr.IsValid()) return  false;
    val=Ptr->ToString().c_str();
    return true;
}
