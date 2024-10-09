#ifndef FINDPICPRO_H
#define FINDPICPRO_H

#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QElapsedTimer>
#include <QThread>

#include "snapShoot.h"

struct CompeteRes
{
    cv::Point point;
    int num;
};

cv::Mat convertToGray(const cv::Mat& inputImage);

// 将 QImage 转换为 cv::Mat
cv::Mat QImageToMat(const QImage &image);

cv::Point templateMatch(const cv::Mat& screenImage, const cv::Mat& templateImage, double threshold);

// 支持.png格式的资源文件图片
cv::Point findPicInFullScreen(QString picPath);

// 循环找图
cv::Point findPicInLimitedTime(QString picPath, int timeLimit = 3000, int interval = 1000);

// 分情况找图，返回找到的图片点及图片序号
CompeteRes competeFindPic(QStringList picList, int timeLimit = 3000, int interval = 1000);

#endif // FINDPICPRO_H
