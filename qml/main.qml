import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1

ApplicationWindow {

    id: app

    visible: true
    width: 1000
    height: 700

    property int fontSize: 18
    property color primaryColor: Material.color(Material.Teal)
    property color colorShade100: Material.color(Material.Teal, Material.Shade100)
    property color colorShade700: Material.color(Material.Teal, Material.Shade700)

    TabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: app.fontSize * 3


       TabButton {
           text: "Archers / Qualifications"
           font.pixelSize: fontSize*0.9
       }
       TabButton {
           text: "Équipes"
           font.pixelSize: fontSize*0.9
       }
       TabButton {
           text: "Matches"
           font.pixelSize: fontSize*0.9
       }
       TabButton {
           text: "Réglages"
           font.pixelSize: fontSize*0.9
       }
    }

    ArcherTab {
        id: archerTab
        visible: tabBar.currentIndex == 0
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right


    }



}
