#ifndef MOUSEKEYBORADCONTROL_H
#define MOUSEKEYBORADCONTROL_H

#include <QApplication>
#include <QCursor>
#include <windows.h>
#include <QString>

// 模拟鼠标移动
void MoveMouse(int x, int y) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN); // 转换为屏幕坐标
    input.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN); // 转换为屏幕坐标
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

// 模拟鼠标左键点击
void LeftClick() {
    INPUT input[2] = {0};

    // 按下右键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // 松开右键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, input, sizeof(INPUT));
}

// 模拟鼠标右键点击
void RightClick() {
    INPUT input[2] = {0};

    // 按下右键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    // 松开右键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    SendInput(2, input, sizeof(INPUT));
}


// 模拟鼠标中键点击
void MiddleClick() {
    INPUT input[2] = {0};

    // 按下中键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

    // 松开中键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

    SendInput(2, input, sizeof(INPUT));
}


// 模拟鼠标滚轮滚动
void ScrollMouse(int amount) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = amount;  // 正值向上滚动，负值向下滚动

    SendInput(1, &input, sizeof(INPUT));
}

//
void mouseClick(int x, int y){
    MoveMouse(x, y);
    LeftClick();
    MoveMouse(1, 1);
}

#endif // MOUSEKEYBORADCONTROL_H
