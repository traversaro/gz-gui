import QtQuick 2.0
import QtQuick.Controls 2.0
import RenderWindow 1.0

Rectangle {
  objectName: "rect"
  width: 1000
  height: 800
  color: "blue"

  RenderWindow {
    id: renderWindow
    objectName: "rw"
    anchors.fill: parent
    anchors.margins: 20
  }
}


