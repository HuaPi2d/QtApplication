#include "snapShoot.h"

QPixmap captureFullScreen() {
    // 获取当前屏幕
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
        return QPixmap();

    // 截图
    QPixmap pixmap = screen->grabWindow(0);

    return pixmap;

    // 保存截图到文件
    // QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Screenshot", QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".png", "Images (*.png *.jpg *.bmp)");
    // if (!filePath.isEmpty()) {
    //     pixmap.save(filePath);
    // }
}
