import QtQuick 2.11
import com.PylonFeaturesEnum 1.0
import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    id: pylonComboBox
    property int parametername: -1
    property var link: null
    property int curValue: mCombo.curValue
    property var customModel: ["1","2", "3"]


    ComboBoxPanel{
        id: mCombo
        anchors.fill: parent
        updateValueFunction: updateValue
        customModel: pylonComboBox.customModel
    }

    property int val: -1
    function initializionVal()
    {
        if (parametername===-1) return
        mCombo.panelName=pn.getNameRU(parametername,PylonFeaturesEnum.DEnum)
       // mCombo.hint=pn.getHint(parametername,PylonFeaturesEnum.Enum)
        val=myCamera.getEnumCurrentValueToInt(parametername)
        if (val<0 && val>=mCombo.model.count) return
        mCombo.curValue=val
    }

    function updateValue()
    {
        if (parametername===-1 )
        {
            if (link) link()
            return
        }
        if (!myCamera.setEnumFeatures(parametername,mCombo.curValue))
        {
            initializionVal()
        }
        if (link) link()
    }



}
