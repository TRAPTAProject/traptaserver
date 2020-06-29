import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.1

import "../spreadsheet"


Item {
    id: archerTab

    Item {
        id: archerTabSettings
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        height: app.fontSize*3.5

        Label {
            id: serieLabel
            font.pixelSize: app.fontSize
            color: "black"
            text: "Nb séries :"
            anchors.right: serieCombobox.left
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: serieCombobox
            font.pixelSize: app.fontSize
            model: ["1", "2", "4"]
            anchors.right: volleyLabel.left
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            width: app.fontSize*5

        }
        Label {
            id: volleyLabel
            font.pixelSize: app.fontSize
            color: "black"
            text: "Nb volées par série :"
            anchors.right: volleyCombobox.left
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: volleyCombobox
            font.pixelSize: app.fontSize
            model: ["3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "999"]
            anchors.right: arrowLabel.left
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            width: app.fontSize*5
        }
        Label {
            id: arrowLabel
            font.pixelSize: app.fontSize
            color: "black"
            text: "Nb flèches par volée :"
            anchors.right: arrowCombobox.left
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
        }
        ComboBox {
            id: arrowCombobox
            font.pixelSize: app.fontSize
            model: ["3", "6"]
            anchors.right: x10Checkbox.left
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            width: app.fontSize*5
        }
        CheckBox {
            id: x10Checkbox
            text: "10X"
            font.pixelSize: app.fontSize
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter

        }

        ToolButton {
            id: toolButton
            text: "\u2630"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: app.fontSize*1.5
            onClicked: drawer.open();

        }

        Label {
            id: menuLabel
            font.pixelSize: app.fontSize
            color: "red"
            text: "MENU"

            anchors.verticalCenter: parent.verticalCenter
            visible: opacity>0
            x: parent.width

            SequentialAnimation {
                running: true
                NumberAnimation { target: menuLabel; property: "x"; to:(toolButton.x+toolButton.width+5); easing.type: Easing.OutCubic; duration:1000 }
                NumberAnimation { target: menuLabel; property: "opacity"; from:1; to:0; duration:200 }
                PauseAnimation { duration: 100 }
                NumberAnimation { target: menuLabel; property: "opacity"; from:1; to:0; duration:200 }
                PauseAnimation { duration: 100 }
                NumberAnimation { target: menuLabel; property: "opacity"; from:1; to:0; duration:200 }
                PauseAnimation { duration: 100 }
                NumberAnimation { target: menuLabel; property: "opacity"; from:1; to:0; duration:5000 }
            }

        }


    }

    Rectangle {
        id: roundSelectorBg
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: archerTabSettings.bottom
        height: roundSelector.height
        color: Material.primary

        ComboBox {
            id: roundSelector
            model: ["Départ 1", "Départ 2", "Départ 3"]
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.top: parent.top
            width: 16*app.fontSize
            font.pixelSize: app.fontSize
        }

    }


    SpreadSheet {
        id: spreadSheet
        spreadSheetModel: archerListModel
        anchors.top: roundSelectorBg.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        fontSize: app.fontSize
        primaryColor: app.primaryColor
        colorShade100: app.colorShade100
        colorShade700: app.colorShade700
        headerHeight: spreadSheet.fontSize*2
        rowHeight: spreadSheet.fontSize*2
        rowHighlightHovering: true


    }

    Drawer {
        id: drawer
        edge: Qt.LeftEdge
        width: app.fontSize * 17
        height: archerTab.height
        y: archerTab.y


        ListView {
            id: drawerListView
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: aboutListView.top
            focus: true

            model: ListModel {
                ListElement { functionName: "Effacer tous les departs"; funtionId: 0 }
                ListElement { functionName: "Fonction 2"; functionId: 1 }
            }

            delegate: MenuRow {
                width: parent.width
                fontSize: app.fontSize
                selected: index === drawerListView.currentIndex
                text: functionName
                onHovered: {
                    aboutListView.currentIndex = -1
                    drawerListView.currentIndex = index
                }
                onClicked: {
                    print("Click menu id "+functionId)
                    drawer.close();
                    switch (functionId) {
                        case 0: {

                            break
                        }
                        case 1: {

                            break
                        }


                    }
                }
            }

        }



        ListView {
            id: aboutListView

            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            height: contentHeight

            focus: true

            model: ListModel {
                ListElement { functionName: "A propos"; functionId: 5 }
                ListElement { functionName: "Quitter"; functionId: 6 }
            }

            delegate: MenuRow {
                width: parent.width
                fontSize: app.fontSize
                selected: index === aboutListView.currentIndex
                text: functionName
                onHovered: {
                    drawerListView.currentIndex = -1
                    aboutListView.currentIndex = index
                }
                onClicked: {
                    print("Click menu id "+functionId)
                    drawer.close();
                    switch (functionId) {
                        case 5: {

                            //viewcontroller.about();
                            break
                        }
                        case 6: {

                            //viewcontroller.quit();
                            break;
                        }

                    }
                }
            }

        }

    }



}
