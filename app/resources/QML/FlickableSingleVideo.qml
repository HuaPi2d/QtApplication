import QtQuick 2.15
import QtQuick.Controls 2.15
import QMLVideoInfo 1.0
import QMLVideoInfoListModel 1.0
import Qt5Compat.GraphicalEffects

Item {
    anchors.fill: parent

    Flickable{
        id: flickable
        anchors.fill: parent
        contentWidth: parent.width

        interactive: false

        signal scrollToBottom();

        Flow{
            id: flow
            width: parent.width

            Repeater{
                id: videoList
                model: videoInfoListModel

                signal itemClicked(int index)

                Rectangle{
                    width: 300
                    height: 265

                    color: widgetColor

                    Rectangle {
                        id: video
                        anchors.fill: parent
                        anchors.margins: 10
                        color: widgetColor

                        Image{
                            id: cover
                            visible: false

                            width: parent.width
                            height: parent.width / 1.75

                            source: pic

                            Rectangle{
                                anchors.bottom: parent.bottom
                                width: parent.width
                                height: 30

                                gradient: Gradient{
                                    GradientStop{position: 0.0; color: "#00FFFFFF"}
                                    GradientStop{position: 1.0; color: "#AC000000"}
                                }

                                Image{
                                    id: playIcon
                                    height: parent.height - 8
                                    width: height
                                    anchors.left: parent.left
                                    anchors.leftMargin: 6
                                    anchors.top: parent.top
                                    anchors.topMargin: 4

                                    source: "qrc:/icon/resources/icons/Video Clip.png"
                                }

                                Text{
                                    id: playTimes
                                    anchors.left: playIcon.right
                                    anchors.leftMargin: 3
                                    anchors.top: parent.top
                                    anchors.topMargin: 7

                                    text: play
                                    font.pixelSize: 13
                                    color: "white"
                                    font.bold: true
                                }

                                Text{
                                    id: durationText
                                    anchors.right: parent.right
                                    anchors.rightMargin: 8
                                    anchors.top: parent.top
                                    anchors.topMargin: 7

                                    text: duration
                                    font.pixelSize: 13
                                    color: "white"
                                    font.bold: true
                                }
                            }

                            Image{
                                width: 50
                                height: 50
                                id: previewIcon
                                anchors.centerIn: parent
                                source: "qrc:/icon/resources/icons/bilibili.svg"
                            }

                            onStatusChanged: {
                                if(cover.status === Image.Loading){
                                    previewIcon.visible = true
                                }
                                else{
                                    previewIcon.visible = false
                                }
                            }
                        }

                        Rectangle{
                            id: maskRec
                            visible: false

                            width: parent.width
                            height: parent.width / 1.75

                            radius: 7
                        }

                        OpacityMask{
                            id: finalImage

                            anchors.fill: cover
                            source: cover
                            maskSource: maskRec

                            Behavior on scale {
                                NumberAnimation {
                                    duration: 200
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        }

                        Text{
                            id: titleName
                            width: parent.width

                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: cover.bottom
                            anchors.topMargin: 13

                            text: title
                            font.pixelSize: 16

                            elide: Text.ElideRight  // 超出部分在右边显示省略号
                            wrapMode: Text.WordWrap  // 自动换行
                            maximumLineCount: 2  // 最多显示两行
                        }

                        onColorChanged: {
                            if(widgetColor.r > 0.5){
                                titleName.color = "black"
                            }
                            else{
                                titleName.color = "white"
                            }
                        }

                        Rectangle{
                            id: bottomInfo
                            width: parent.width
                            height: 20
                            anchors.bottom: parent.bottom
                            color: widgetColor

                            Image{
                                id: upIcon
                                width: 20
                                height: 20
                                anchors.top: parent.top

                                source: "qrc:/icon/resources/icons/UP.svg"
                            }

                            Text{
                                id: authorAndDate
                                anchors.top: parent.top
                                anchors.left: upIcon.right
                                anchors.leftMargin: 5

                                text: author + " · " + pubdate
                                font.pixelSize: 15
                                color: "#8a8a8a"
                            }
                        }

                        MouseArea{
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                finalImage.scale = 1.03
                                titleName.color = "#0eb2ed"
                            }

                            onExited: {
                                finalImage.scale = 1.0
                                if(widgetColor.r > 0.5){
                                    titleName.color = "black"
                                }
                                else{
                                    titleName.color = "white"
                                }
                            }

                            onPressed: {
                                finalImage.scale = 1.0
                                titleName.color = "#0eb2ed"
                            }

                            onReleased: {
                                finalImage.scale = 1.03
                                titleName.color = "#0eb2ed"
                            }

                            // 点击事件打开相应视频
                            onClicked:{
                                videoList.itemClicked(index)
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    itemClicked.connect(videoInfoListModel.itemClicked)
                }
            }
        }

        Behavior on contentY {
            NumberAnimation {
                duration: 100  // 动画持续时间，单位是毫秒
                easing.type: Easing.OutQuad  // 使用缓动效果
            }
        }

        WheelHandler {

            // 处理纵向滚动
            onWheel: (event)=>{
                if(flickable.contentY >= 0 && flickable.contentY <= flow.height - flickable.height){

                    flickable.contentY -= event.angleDelta.y
                }
                else if(flickable.contentY < 0){
                    flickable.contentY = 0
                }
                else if(flickable.contentY > flow.height - flickable.height){
                    flickable.contentY = flow.height - flickable.height
                    if(videoInfoListModel.state == 0){
                        flickable.scrollToBottom()
                    }
                }
            }
        }

        Component.onCompleted: {
            scrollToBottom.connect(videoInfoListModel.scrollToBottom)
        }

    }

    VideoInfoListModel{
        id: videoInfoListModel
        objectName: "videoInfoListModel"
    }

}
