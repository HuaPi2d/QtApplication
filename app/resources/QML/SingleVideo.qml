import QtQuick 2.15
import QMLVideoInfo 1.0
import QMLVideoInfoListModel 1.0

Item {

    Flow{
        id: flow
        width: parent.width
        height: parent.height
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Repeater{
            id: videoList
            model: videoInfoListModel


            Column {
                spacing: 10

                Image {
                    source: pic
                    width: 290
                    height: 166

                    Rectangle{

                        width: parent.width
                        height: 33
                        anchors.bottom: parent.bottom

                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#00FFFFFF" }  // 上部颜色浅，透明度高
                            GradientStop { position: 1.0; color: "#AC000000" }  // 下部颜色深，透明度低
                        }

                        Row {
                            width: parent.width
                            spacing: 5

                            Image{
                                height: 30
                                width: 30
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                source: "qrc:/icon/resources/icons/audio.svg"
                            }
                        }
                    }

                }
            }
        }
    }

    VideoInfoListModel
    {
        id: videoInfoListModel
        objectName: "videoInfoListModel"
    }


}
