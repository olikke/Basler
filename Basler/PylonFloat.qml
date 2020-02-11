import QtQuick 2.11
import com.PylonFeaturesEnum 1.0
import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    id: pylonFloat
    property int parametername: -1
    property var  link: null

    Row {
        id: row
        spacing: 2
        anchors.margins: 0
        anchors.fill: parent
        property double loLimit: 0
        property double hiLimit :0

        Rectangle {
            width: parent.width/2
            color: Style.currentTheme.background
            height: parent.height
            radius: Style.unitRadius /2
            CustomLabel{
                id: cl
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: parent.width
                horizontalAlignment: Text.AlignLeft
            }
        }

        Rectangle {
            width: parent.width/2
            color: Style.currentTheme.background
            height: parent.height
            radius: Style.unitRadius /2
            CustomTextField{
                id: tf
                enabled: true
                anchors.left: parent.left
                anchors.leftMargin: 66
                anchors.right: parent.right
                anchors.rightMargin: 10
                height: parent.height
                horizontalAlignment: Text.AlignRight
                validator: DoubleValidator {bottom:row.loLimit; top: row.hiLimit}
                onAccepted: updateValue()
            }
        }
    }

    function updateValue()
    {
        if (parametername===-1) return
        if (!myCamera.setFloatFeatures(parametername,parseFloat(tf.text)))
        {
            initializionVal()
        }
        if (link) link()
    }

    function initializionVal()
    {
        if (parametername===-1) return
        pn.getNameRU(parametername,PylonFeaturesEnum.DFloat)
        cl.text=pn.getNameRU(parametername,PylonFeaturesEnum.DFloat)
        var IntArray=myCamera.getFloatFeatures(parametername)
        if (IntArray[0]===0)
        {
            tf.enabled=false
            tf.text="0.0"
            return
        }
        tf.text=IntArray[3].toFixed(1).toString()
        row.loLimit=IntArray[1]
        row.hiLimit=IntArray[2]
    }

}
