import QtQuick 2.11
import com.PylonFeaturesEnum 1.0
import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    id: pylonIntegerSlider
    property int parametername: -1
    property var  link: null

    IntegerSliderPanel{
        id: mSlider
        anchors.fill: parent
        updateValueFunction: updateValue
    }

    function initializionVal()
    {
        if (parametername===-1) return
        mSlider.panelName=pn.getNameRU(parametername,PylonFeaturesEnum.DInt)
        //mSlider.hint=pn.getHint(parametername,PylonFeaturesEnum.DInt)
        var IntArray=myCamera.getIntPtrFeatures(parametername)
        if (IntArray[0]===0)
        {
            mSlider.value=0
            mSlider.enabled=false
            return
        }
        mSlider.minValue=IntArray[1]
        mSlider.maxValue=IntArray[2]
        mSlider.curValue=IntArray[3]
        mSlider.step=IntArray[4]
    }

    function updateValue(value)
    {
        //нужно для перекрестных изменений
        if (link) link()
        if (parametername===-1) return
        if (!myCamera.setIntPtrFeatures(parametername,value))
        {

            initializionVal()
        }

    }



}
