import QtQuick 1.0
import com.nokia.meego 1.0
import Qt.labs.folderlistmodel 1.0


Dialog {
  id: directorySelector
  width : parent.width - 30
  property Style platformStyle: SelectionDialogStyle {}

  property variant filterList: [ "abcd98765" ]

  property string titleText: "Choose directory"
  property variant folderModel: folderModel1
  property variant view: view1
  property string selectedFile: "";

  // go down one view
  function down(path) {
    // slide current view out to the left
    if (folderModel == folderModel1) {
      view = view2
      folderModel = folderModel2;
      view1.state = "exitLeft";
    } else {
      view = view1
      folderModel = folderModel1;
      view2.state = "exitLeft";
    }

    // and slide new view in from right
    view.x = directorySelector.width;
    view.state = "current";
    view.focus = true;
    folderModel.folder = path;
  }

  function up() {
    selectedFile = folderModel.folder;

    var path = folderModel.parentFolder;
    if (folderModel == folderModel1) {
      view = view2
      folderModel = folderModel2;
      view1.state = "exitRight";
    } else {
      view = view1
      folderModel = folderModel1;
      view2.state = "exitRight";
    }
    view.x = -directorySelector.width;
    view.state = "current";
    view.focus = true;
    folderModel.folder = path;
  }

  //property Component delegate:

  Component {
    id: defaultDelegate

    Item {
      id: delegateItem
      property bool selected: filePath === selectedFile;

      height: directorySelector.platformStyle.itemHeight
      anchors.left: parent.left
      anchors.right: parent.right

      //visible: folderModel.isFolder(index)

      MouseArea {
        id: delegateMouseArea
        anchors.fill: parent;
        onPressed: selectedFile = filePath;
        onClicked:  {
          if (folderModel.isFolder(index))
             down(filePath);
        }
      }

      Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: delegateItem.selected ? directorySelector.platformStyle.itemSelectedBackgroundColor : directorySelector.platformStyle.itemBackgroundColor
      }

      BorderImage {
        id: background
        anchors.fill: parent
        border { left: 22; top: 2; right: 2; bottom: 22 }
        source: delegateMouseArea.pressed ? directorySelector.platformStyle.itemPressedBackground :
        delegateItem.selected ? directorySelector.platformStyle.itemSelectedBackground :
                directorySelector.platformStyle.itemBackground
      }

      Text {
        id: itemText
        elide: Text.ElideRight
        color: delegateItem.selected ? directorySelector.platformStyle.itemSelectedTextColor : directorySelector.platformStyle.itemTextColor
        anchors.verticalCenter: delegateItem.verticalCenter
        anchors.left: parent.left
        anchors.right: folderModel.isFolder(index)?downArrow.left:parent.right
        anchors.leftMargin: directorySelector.platformStyle.itemLeftMargin
        anchors.rightMargin: directorySelector.platformStyle.itemRightMargin
        text: fileName
        font: directorySelector.platformStyle.itemFont
      }

      // add "right" arrow to all directories
      Image {
        id: downArrow
        source: "image://theme/icon-m-common-drilldown-arrow-inverse"
        anchors.right: parent.right;
        anchors.verticalCenter: parent.verticalCenter
        visible: folderModel.isFolder(index)
      }
    }
  }

  title: Item {
    id: header
    height: directorySelector.platformStyle.titleBarHeight

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    Item {
      id: labelField

      anchors.fill:  parent

      Item {
        id: labelWrapper
        anchors.left: parent.left
        anchors.right: closeButton.left

        anchors.bottom:  parent.bottom
        anchors.bottomMargin: directorySelector.platformStyle.titleBarLineMargin

        height: titleLabel.height

        Label {
          id: titleLabel
          x: directorySelector.platformStyle.titleBarIndent
          width: parent.width - closeButton.width
          font: directorySelector.platformStyle.titleBarFont
          color: directorySelector.platformStyle.commonLabelColor
          elide: directorySelector.platformStyle.titleElideMode
          text: directorySelector.titleText
        }
      }

      Image {
        id: closeButton
        anchors.bottom:  parent.bottom
        anchors.bottomMargin: directorySelector.platformStyle.titleBarLineMargin-6
        anchors.right: labelField.right

        opacity: closeButtonArea.pressed ? 0.5 : 1.0
        source: "image://theme/icon-m-common-dialog-close"

        MouseArea {
          id: closeButtonArea
          anchors.fill: parent
          onClicked:  {directorySelector.reject();}
        }
      }
    }

    Rectangle {
      id: headerLine

      anchors.left: parent.left
      anchors.right: parent.right

      anchors.bottom:  header.bottom

      height: 1

      color: "#4D4D4D"
    }
  }

  content: Item {
    id: contentField

    property int maxListViewHeight : visualParent ? visualParent.height * 0.87
        - directorySelector.platformStyle.titleBarHeight
        - directorySelector.platformStyle.contentSpacing - 50
       : directorySelector.parent ? directorySelector.parent.height * 0.87
        - directorySelector.platformStyle.titleBarHeight
        - directorySelector.platformStyle.contentSpacing - 50
     : 350

    height: maxListViewHeight
    width: directorySelector.width
    y : directorySelector.platformStyle.contentSpacing
    clip: true

    // we have two list views and are shifting them left and right
    // for nice animation

    Item {
      id: pathItem
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top:  parent.top
      height: directorySelector.platformStyle.itemHeight

      property bool canGoUp: folderModel.parentFolder != "" && folderModel.folder != folderModel.parentFolder

      enabled: canGoUp
      MouseArea {
        id: backArea
        anchors.fill: parent
        onClicked:  { up(); }
      }

      // add "left" arrow to go up one directory
      Image {
        id: backButton
        //source: "image://theme/icon-m-startup-back"
        // fix Fremantle CSSU icon availability

        //source: platform.incompleteTheme() ? "image://theme/icon-m-toolbar-back-white-selected" :
        //"image://theme/icon-m-startup-back"

        source :"image://theme/icon-m-startup-back"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        opacity: parent.canGoUp?(backArea.pressed ? 0.5 : 1.0):0.4
      }

      Label {
        id: pathLabel
        font: directorySelector.platformStyle.itemFont
        color: directorySelector.platformStyle.itemTextColor
        elide: Text.ElideLeft

        text: folderModel.folder

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: backButton.right
        anchors.right: parent.right

        anchors.leftMargin: directorySelector.platformStyle.itemLeftMargin
        anchors.rightMargin: directorySelector.platformStyle.itemRightMargin
      }

      Rectangle {
        id: pathLine

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.bottom:  parent.bottom

        height: 1

        color: "#4D4D4D"
      }
    }

    Item {
      clip: true
      focus: true

      anchors.top: pathItem.bottom
      anchors.bottom: parent.bottom
      width: parent.width

      ListView {
        id: view1
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: 0
        width: parent.width
        pressDelay: directorySelector.platformStyle.pressDelay

        FolderListModel {
          id: folderModel1
          nameFilters: filterList
        }

        model: folderModel1
        delegate: defaultDelegate

        states: [
          State {
            name: "current"
            PropertyChanges { target: view1; x: 0 }
          },
          State {
            name: "exitLeft"
            PropertyChanges { target: view1; x: -parent.width }
          },
          State {
            name: "exitRight"
            PropertyChanges { target: view1; x: parent.width }
          }
        ]
        transitions: [
          Transition {
            NumberAnimation { properties: "x"; duration: 250 }
          }
        ]
      }

      ListView {
        id: view2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        x: parent.width
        width: parent.width
        pressDelay: directorySelector.platformStyle.pressDelay

        FolderListModel {
          id: folderModel2
          nameFilters: filterList
        }

        model: folderModel2
        delegate: defaultDelegate

        states: [
          State {
            name: "current"
            PropertyChanges { target: view2; x: 0 }
          },
          State {
            name: "exitLeft"
            PropertyChanges { target: view2; x: -parent.width }
          },
          State {
            name: "exitRight"
            PropertyChanges { target: view2; x: parent.width }
          }
        ]
        transitions: [
          Transition {
            NumberAnimation { properties: "x"; duration: 250 }
          }
        ]
      }
    }

    Item {
        id: selectFolder
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom:  parent.bottom
        height: directorySelector.platformStyle.itemHeight

        Rectangle {
            anchors.fill: parent
            color: "black"

            MouseArea {
                id: selectFolderArea
                anchors.fill: parent
                onClicked:  { accept(); }
            }

            Label {
                id: selectFolderLabel
                font: directorySelector.platformStyle.itemFont
                color: directorySelector.platformStyle.itemTextColor
                elide: Text.ElideLeft

                text: "Select folder"

                anchors.centerIn: parent


                anchors.leftMargin: directorySelector.platformStyle.itemLeftMargin
                anchors.rightMargin: directorySelector.platformStyle.itemRightMargin
            }

//            Rectangle {
//                id: selectFolderLine

//                anchors.left: parent.left
//                anchors.right: parent.right

//                anchors.top:  parent.top

//                height: 1

//                color: "#4D4D4D"
//            }
        }
    }


    MouseArea {
      property int xPos
      enabled: false

      anchors.fill: parent
      onPressed: { console.log("Pressed"); xPos = mouseX; mouse.accepted = false; }
      onReleased: {
        console.log("Swipe: " + mouseX + " " +xPos);
        if (mouseX - xPos > width/5) {
          up();
          mouse.accepted = false;
        }
      }
    }
  }
}
