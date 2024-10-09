#include "ssjjCoreFun.h"
#include "universal/mouseKeyboradControl.h"

cv::Point findAndClick(QString picPath, int timeLimit, int interval)
{
    cv::Point point = findPicInLimitedTime(picPath, timeLimit, interval);
    QThread::msleep(1000);
    if(point == cv::Point(-1, -1)){
        return point;
    }
    else{
        mouseClick(point.x, point.y);
        QThread::msleep(1000);
        return point;
    }
}

cv::Point findAndClickAndCheck(QString picPath, int waitTime, int timeLimit, int interval){
    cv::Point res = findAndClick(picPath, timeLimit, interval);
    QThread::msleep(waitTime);
    if(findPicInFullScreen(picPath) != cv::Point(-1, -1)){
        res = findAndClick(picPath, timeLimit, interval);
    }
    return res;
}

SSJJRunState restartSSJJ(QString ssjjInstallPath){
    SSJJRunState ssjjRunState;

    TerminateProcessByName("SSJJ_BattleClient_Unity.exe");
    TerminateProcessByName("MicroClient.exe");

    // int times = 30;
    while(true){
        if(findProcessByName("MicroClient.exe") != DWORD() || findProcessByName("SSJJ_BattleClient_Unity.exe") != DWORD()){
            TerminateProcessByName("SSJJ_BattleClient_Unity.exe");
            TerminateProcessByName("MicroClient.exe");
        }
        else{
            break;
        }
        QThread::msleep(100);
        // times--;
        // if(times == 0){
        //     ssjjRunState.remindText = "<p>请开启<span style=\"color: red;\"><b>自动登录</b></span></p><br>";
        //     ssjjRunState.errorType = "FatalError";
        //     return ssjjRunState;
        // }
    }

    for(int i = 0; i < 30; i++){
        if(findProcessByName("GameMon.des") == DWORD() && findProcessByName("GameGuard.des") == DWORD() && findProcessByName("GameMon64.des") == DWORD()){
            break;
        }
        QThread::msleep(1000);
    }
    runProgramAsAdmin(ssjjInstallPath, QStringList());
    QStringList picList;
    picList.append(":/pic/script/resources/pic/script/loaded_cover.png");
    picList.append(":/pic/script/resources/pic/script/first_server.png");
    CompeteRes res = competeFindPic(picList, 60000, 1000);
    if(res.num == 0){
        ssjjRunState.remindText = "<p>请开启<span style=\"color: red;\"><b>自动登录</b></span></p><br>";
        ssjjRunState.errorType = "FatalError";
        return ssjjRunState;
    }
    else if(res.num == -1){
        ssjjRunState.remindText = "<p>发生未知错误，请检查<span style=\"color: red;\"><b>网络连接</b></span></p><br>";
        ssjjRunState.errorType = "Error";
        return ssjjRunState;
    }
    mouseClick(res.point.x, res.point.y);

    /* 寻找是否登录 */
    picList.clear();
    picList.append(":/pic/script/resources/pic/script/close_activity.png");
    picList.append(":/pic/script/resources/pic/script/hall.png");
    picList.append(":/pic/script/resources/pic/script/money.png");
    res = competeFindPic(picList, 60000, 1000);
    if(res.num == -1){
        ssjjRunState.remindText = "<p>发生未知错误，请检查<span style=\"color: red;\"><b>网络连接</b></span></p><br>";
        ssjjRunState.errorType = "Error";
        return ssjjRunState;
    }
    mouseClick(2, 2);
    QThread::msleep(8000);

    /* 判断是否弹出活动界面 */
    res = competeFindPic(picList, 3000, 1000);

    /* 如果有，退出活动界面 */
    if(res.num == 0){
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 3000);
        QThread::msleep(8000);
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 3000);
    }
    /* 如果没有，进行任意操作后弹出活动界面 */
    else if(res.num == 1){
        findAndClick(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
        QThread::msleep(5000);
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/leisure_mode.png", 3000) != cv::Point(-1,-1)){
            findAndClick(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
        }

        /* 此时弹出活动界面 */
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 10000);
    }

    if(findPicInLimitedTime(":/pic/script/resources/pic/script/hall.png") != cv::Point(-1, -1))
    {
        ssjjRunState.remindText = "<p>已重启应用</p><br>";
        ssjjRunState.errorType = "NoError";
        return ssjjRunState;
    }
    else{
        ssjjRunState.remindText = "<p>初始化失败</p><br>";
        ssjjRunState.errorType = "Error";
        return ssjjRunState;
    }
}

SSJJRunState initiallizeGameScreen(QString taskName){
    SSJJRunState ssjjRunState;
    QStringList picList;
    CompeteRes res;
    cv::Point point;

    /* 回到大厅 */
    point = findAndClick(":/pic/script/resources/pic/script/hall.png", 6000);
    if(point == cv::Point(-1, -1)){
        ssjjRunState.errorType = "Error";
        ssjjRunState.remindText = "<p>未检测到游戏画面</p><br>";
        return ssjjRunState;
    }
    findAndClick(":/pic/script/resources/pic/script/confirm_exit_channel.png", 3000);
    if(taskName == "团队道具赛"){
        findAndClickAndCheck(":/pic/script/resources/pic/script/leisure_mode.png", 3000);

        findAndClick(":/pic/script/resources/pic/script/LD_Icon.png", 8000);
        picList.clear();
        picList.append(":/pic/script/resources/pic/script/cancel.png");
        picList.append(":/pic/script/resources/pic/script/energy_gray.png");
        picList.append(":/pic/script/resources/pic/script/energy_lighted.png");
        res = competeFindPic(picList, 3000, 1000);
        // if(res.num == -1){
        //     ssjjRunState.errorType = "Error";
        //     ssjjRunState.remindText = "<p>未检测到游戏画面</p><br>";
        //     return ssjjRunState;
        // }
        // 上场战斗未结束
        if(res.num == 0 || res.num == -1){
            picList.clear();
            picList.append(":/pic/script/resources/pic/script/cancel.png");
            picList.append(":/pic/script/resources/pic/script/energy_gray.png");
            picList.append(":/pic/script/resources/pic/script/energy_lighted.png");
            for(int i = 0; i < 10; i++){
                findAndClick(":/pic/script/resources/pic/script/cancel.png", 3000);
                QThread::msleep(30000);
                point = findAndClick(":/pic/script/resources/pic/script/hall.png", 3000);
                if(point == cv::Point(-1, -1)){
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "<p>未检测到游戏画面</p><br>";
                    return ssjjRunState;
                }
                findAndClick(":/pic/script/resources/pic/script/confirm_exit_channel.png", 3000);
                findAndClick(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
                findAndClick(":/pic/script/resources/pic/script/LD_Icon.png", 8000);
                CompeteRes res = competeFindPic(picList);
                if(res.num == -1){
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "<p>发生未知错误</p><br>";
                    return ssjjRunState;
                }
                else if(res.num != 0){
                    break;
                }
            }
        }
        picList.clear();
        picList.append(":/pic/script/resources/pic/script/energy_gray.png");
        picList.append(":/pic/script/resources/pic/script/energy_lighted.png");
        CompeteRes res = competeFindPic(picList);
        if(res.num == -1){
            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "<p>发生未知错误</p><br>";
            return ssjjRunState;
        }
        else if(res.num == 0){
            mouseClick(res.point.x, res.point.y);
        }
        else{
            mouseClick(res.point.x, res.point.y);
            mouseClick(res.point.x, res.point.y);
            mouseClick(res.point.x, res.point.y);
        }
        ssjjRunState.errorType = "NoError";
        ssjjRunState.remindText = "<p>初始化成功</p><br>";
        return ssjjRunState;
    }
    else{
        return ssjjRunState;
    }
}

SSJJRunState enterGame(SingleTask task, int loadingTimes){
    SSJJRunState ssjjRunState;
    cv::Point point;

    if(task.taskName == "团队道具赛"){
        findAndClickAndCheck(":/pic/script/resources/pic/script/LD_begin.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/morningRemind.png", 3000);
        QThread::msleep(loadingTimes);
        for(int i = 0; i < 20; i++){
            point = findPicInFullScreen(":/pic/script/resources/pic/script/loading_pic.png");
            DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
            if(point == cv::Point(-1, -1) && handle != DWORD()){
                ssjjRunState.errorType = "NoError";
                ssjjRunState.remindText = "";
                return ssjjRunState;
            }
            else if(point == cv::Point(-1, -1) && handle == DWORD()){
                // 卡前加载页面
            }
            QThread::msleep(2000);
        }

        // 检查逃跑
        TerminateProcessByName("SSJJ_BattleClient_Unity.exe");
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/exit_LD.png") != cv::Point(-1, -1)){
            findAndClick(":/pic/script/resources/pic/script/exit_LD.png");
            ssjjRunState.errorType = "Remind";
            ssjjRunState.remindText = "";
            return ssjjRunState;
        }

        ssjjRunState.errorType = "Error";
        ssjjRunState.remindText = "<p><span style=\"color: red;\"><b>长时间停留在加载界面</b></span>，准备重启游戏</p><br>";
        return ssjjRunState;
    }
    else{
        return ssjjRunState;
    }
}

SSJJRunState runScript(SingleTask task, int speed)
{
    SSJJRunState ssjjRunState;

    LeftClick();
    if(task.script == "未选择"){
        // 无脚本，如乱斗
        // 检测是否退出游戏
        while(true){
            QThread::msleep(5);
            DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
            if(handle == DWORD()){
                ssjjRunState.errorType = "NoError";
                ssjjRunState.remindText = "";
                return ssjjRunState;
            }
        }
    }
}

SSJJRunState settlement(QString taskName)
{
    SSJJRunState ssjjRunState;
    cv::Point point;

    if(taskName == "团队道具赛"){
        QThread::msleep(5000);
        for(int i = 0; i < 3; i++){
            QThread::msleep(3000);
            findAndClick(":/pic/script/resources/pic/script/use_energy.png", 3000);
        }
        findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/LD_begin.png", 5000) != cv::Point(-1, -1)){
            ssjjRunState.remindText = "";
            ssjjRunState.errorType = "Success";
            return ssjjRunState;
        }
        else{
            ssjjRunState.remindText = "";
            ssjjRunState.errorType = "Error";
            return ssjjRunState;
        }
    }
}
