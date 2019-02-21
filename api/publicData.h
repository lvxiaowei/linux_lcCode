#ifndef PUBLICDATA_H
#define PUBLICDATA_H
#include <QString>
#include <QStringList>
#include <QKeyEvent>
/*sta----------------------------------设备各类配置文件的路径------------------------------------*/
/*Chain File*/
#define PATH_CHAIN_FILE_LOCAL   "/home/root/dualcylind/sta/"
#define PATH_CHAIN_ACTIVE_FILE  "/home/root/dualcylind/sta/20181123_s200.sta"
/*配置文件*/
#define COMMCONFIG_FILE_PATH  "/home/root/dualcylind/commonConfig.xml"
/*usb path*/
#define USB_PATH "/media/sda1"
/*airValve config*/
#define AIRVALUECONFIG_EN       "/home/root/dualcylind/config_STA/inivalve_en.txt"
#define AIRVALUECONFIG_CH       "/home/root/dualcylind/config_STA/inivalve_zh.txt"
/*inSignal config*/
#define SINGANCONFIG_EN         "/home/root/dualcylind/config_STA/Inputsignal_en.txt"
#define SINGANCONFIG_CH         "/home/root/dualcylind/config_STA/Inputsignal_zh.txt"
/*pattern file*/
#define PATTERN_FILE_LOCAL_PATH "/home/root/dualcylind/sta"

#define CONFIG_FILE_XML_PATH        "/home/root/dualcylind/MachineConfig.xml"
/*背光设置路径*/
#define PATH_BACKLIGHT  "/sys/devices/soc0/backlight.16/backlight/backlight.16/brightness"
/*end----------------------------------设备各类配置文件的路径------------------------------------*/

typedef struct objProperty
{
    int       operMode;       /*操作类型：新建、打开*/
    int       fileType;       /*文件类型：链条文件=0、归零文件=1、宏文件=2*/
    QString   fileName;       /*文件名字*/
    QString   strMachineType; /*机器类型*/
    QString   needle;         /*针数*/
    QString   filePath;       /*文件路径*/
}objProperty;

enum PUBLICDATA_H{
    NEW_FILE,
    OPEN_FILE,
    SAVE_FILE,
};

enum FILETYPE{
    CHAINFILE = 0,    /*链条文件*/
    ZEROFILE,         /*归零文件*/
    MACFILE,          /*宏文件*/
};

/*记录配置文件中宏模块的名字和ico的结构体*/
typedef struct proNameAndIco
{
    QString name;
    QString ico;
}proNameAndIco;

/*记录纱嘴相关数据的结构体*/
typedef struct yarnfingerPro
{
    QString     name;    /*名称*/
    int         count;   /*纱嘴数*/
    QStringList types;   /*类型*/
}yarnfingerPro;

/*记录机器类型相关数据的结构体*/
typedef struct machineTypePro
{
    QList<yarnfingerPro> lstYarnFingerPro;   /*记录纱嘴相关数据的结构体*/
    bool  bNeedLKS;                          /*是否有LKS选针器*/
}machineTypePro;

/*记录命令模块相关数据的结构体*/
typedef struct cmdSettingPro
{
    int      index;  /*索引*/
    QString  NO;    /*记录相关的编号，编号与阀号一一对应*/
    QStringList  name;  /*命令的名字*/
    QString  ico;   /*命令的图案*/
    QString  cmdType;   /*当前命令对应的弹出框编号*/
}cmdSettingPro;
/*记录马达参数的结构体*/
typedef struct motorPro
{
    QString block;          /*步段名称*/
    QString startStep;      /*开始步骤*/
    QString endStep;        /*结束步骤*/
    QString startValue;     /*密度开始值*/
    QString endValue;       /*密度结束值*/
    QString steps;          /*变化圈数*/
}motorPro;


/*主链条节点层*/
enum CmdTreeNodeLevel{
    FIRST_LEVEL_NODE = 1,   /*一级节点*/
    SECEND_LEVEL_NODE,      /*二级节点*/
    THIRD_LEVEL_NODE,       /*三级节点*/
    DEFALUT_LEVE_NODE
};

#define CONFIGFILE "commonConfig.xml"
#define DEFAULT    "default"

#define TEMPORARYFORDER "temporaryFolder"

/*SQL相关处理语句*/
/*主链条相关语句*/
#define SQL_CreateMainChainTable       "create table mainChainTable (weavingPosition varchar ( 20 ) NOT NULL, step int NOT NULL, cmdType int NOT NULL, cmdContent varchar ( 30 ) NOT NULL,val1  varchar ( 20 ),val2  varchar ( 20 ),val3  varchar ( 20 ),val4  varchar ( 20 ),val5  varchar ( 20 ),val6  varchar ( 20 ),val7  varchar ( 20 ),val8  varchar ( 20 ),val9 varchar ( 20 ),val10 varchar ( 20 ))"
#define SQL_WriteMainChainTable        "insert into mainChainTable (weavingPosition, step, cmdType, cmdContent, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10) values ( :weavingPosition, :step, :cmdType, :cmdContent, :val1, :val2, :val3, :val4, :val5, :val6, :val7, :val8, :val9, :val10)"
/*选针器相关语句*/
#define SQL_CreateSelectionTable       "create table selectionTable (id int not null, value varvchar (300), primary key (id))"
#define SQL_WriteSelectionTable        "insert into selectionTable (id, value) values ( :id, :value)"
/*密度设置相关语句*/
#define SQL_CreateStitchTable          "create table stitchTable (motoType int not null, No int, stepName varchar (20), startStep int, endStep int, startVal int, endVal int, steps int )"
#define SQL_WriteStitchTable           "insert into stitchTable (motoType, No, stepName, startStep, endStep, startVal, endVal, steps ) values( :motoType, :No, :stepName, :startStep, :endStep, :startVal, :endVal, :steps )"
/*宏相关语句*/
#define SQL_CreateMacroTable           "create table macroTable (name varchar(20), step int, cmdType int , cmdContent varchar (30), needlVal int )"
#define SQL_WriteMacroTable            "insert into macroTable (name, step, cmdType, cmdContent, needlVal) values( :name, :step, :cmdType, :cmdContent, :needlVal )"
/*花纹相关的语句*/
#define SQL_CreatePatternTable         "create table patternTable (patternName varchar(20), selectionName varchar(10), lineVal int, selectionVal varchar (300), YarnfingerVal varchar(10), rep int )"
#define SQL_WritePatternTable          "insert into patternTable (patternName,selectionName,lineVal,selectionVal,YarnfingerVal,rep) values (:patternName,:selectionName,:lineVal,:selectionVal,:YarnfingerVal,:rep)"
#define SQL_CreatePatternTimingsTable  "create table patternTimingsTable (patternName varchar(20), Yarnfinger varchar(10), YarnfingerNo varchar(10), needlIn int, needlOut int)"
#define SQL_WritePatternTimingsTable   "insert into patternTimingsTable (patternName, Yarnfinger, YarnfingerNo, needlIn, needlOut) values (:patternName, :Yarnfinger, :YarnfingerNo, :needlIn, :needlOut)"
/*通用配置*/
#define SQL_CreateCommonCfgTable       "create table commonCfgTable (fidName varchar(20), fidValue varchar(20))"
#define SQL_WriteCommonCfgTable        "insert into commonCfgTable (fidName, fidValue) values (:fidName, :fidValue)"
/*表名字*/
#define TABLE_MAINCHAIN       "mainChainTable"       //主链条表
#define TABLE_SELECTIONS      "selectionTable"       //选针表
#define TABLE_STITCH          "stitchTable"          //密度设置界面
#define TABLE_MACRO           "macroTable"           //宏表
#define TABLE_PATTERN         "patternTable"         //花型
#define TABLE_PATTERNTIMINGS  "patternTimingsTable"  //花纹纱嘴时序表
#define TABLE_COMMONCFG       "commonCfgTable"       //通用配置表

/*链条树上面命令行的操作类型*/
enum CMD_OPER_TYPE
{
    OPER_VALVE = 1,              /*阀操作*/
    OPER_MACRO,                  /*宏*/
    OPER_SPEED,                  /*速度*/
    OPER_ELASTIC_START,          /*橡筋开始*/
    OPER_ELASTIC_END,            /*橡筋结束*/
    OPER_SELECTION,              /*固定选针*/
    OPER_PATTERN_USE,            /*花型调用*/
    OPER_PATTERN_CONTINUE,       /*花型继续*/
    OPER_PATTERN_QUIT,           /*花型退出*/
    OPER_FOOTPATTERN_USE,        /*脚底花型调用----------------弃用，所有花型调用都使用OPER_PATTERN_USE*/
    OPER_FOOTPATTERN_QUIT,       /*脚底花型退出*/
    OPER_STEP_CIRCLE,            /*步循环*/
    OPER_CIRCLE,                 /*循环*/
    OPER_CIRCLE_END,             /*循环结束*/
    OPER_CHAIN_PAUSE,            /*链条暂停*/
    OPER_MINICIRCLE_ENABLE,      /*最小循环使能*/
    OPER_MINICIRCLE_DISABLE,     /*最小循环除能*/
    OPER_OIL,                    /*加油*/
    OPER_FOOT_INCREASE_DCREASE,  /*脚底增减*/
    OPER_TURNING,                /*转向指令*/
    OPER_LOWSPEED_CMD,           /*低速命令*/
    OPER_UPMOTO,                 /*上针筒马达密度归零*/
    OPER_DOWNMOTO,               /*下针筒马达密度归零*/
    OPER_ELASTICMOTO,            /*橡筋马达密度归零*/
    OPER_ZEROING,                /*归零程序调用*/
    OPER_STEPSTOP,               /*步段停机*/
    OPER_CHAIN_END               /*链条结束*/
};

/*键盘上面宏键相关的操作类型*/
enum macroFun
{
    macroFu_0 = 0,       //Key == 0的时候，只作为获取当前气阀状态获取使用
    macroFu_Reset = 1,   //01 强制初始状态
    macroFu_YFALLOUT,    //02 梭子全出
    macroFu_CutterSet,   //03 剪刀抬起
    macroFu_PatSetStop,  //04 选针器停止
    macroFu_TakeDown,    //05 牵拉
    macroFu_AirFeeder,   //06 进线吹气
    macroFu_ManualCmd,   //07 气阀命令
    macroFu_BackLight,   //08 背光
    macroFu_MiniCycle,   //09 快编
    macroFu_EndCycle,    //10 单只自停
    macroFu_LowSpeed,    //11 低速
    macroFu_ChainBlock,  //12 空转
    macroFu_Zeroing,     //13 归零
    macroFu_FanOnOff,    //14 吸风马达
    macroFu_ParaMod,     //15 密度/速度/圈数
    macroFu_Oiler        //16 加油
};

/*各个界面不同操作提示确认窗口索引*/
enum PromptWindowIndex
{
    pop_settting = macroFu_Oiler+1,  //切换到设置界面的提示窗口的索引
    pop_testing,                     //切换到测试界面的提示窗口的索引
    runningState,                    //主界面运行的索引
};


enum messageType
{
    oper_ChainChange=0,
    oper_MachineParaChange,
};

struct PatternFile_Head
{
    char md5[16];              //128位MD5值
    quint16 needles;           //针数
    quint16 rows;              //花型行数
    quint8 machineRoads;       //机器路数,最大16路
    quint8 YF_Amount[16];      //每路对应的纱嘴个数，最大16路
    quint8 colorAmount;        //颜色数量(最多支持256种颜色)
    quint32 palettePos;        //调色板指针位置
    quint32 paletteSize;       //调色板数据区大小
    quint32 patternPos;        //花型主体指针位置
    quint32 patternSize;       //花型主体数据区大小
    quint32 YF_SelectInfoPos;  //纱嘴选择信息指针位置
    quint32 YF_SelectInfoSize; //纱嘴选择信息数据区大小
    quint32 REP_InfoPos;       //REP循环信息指针位置
    quint32 REP_InfoSize;      //REP循环信息数据区大小
};

#define STATUS      "states"
/*运行指示灯的点灯状态*/
#define STATUS_ON   "ON"
#define STATUS_OFF  "OFF"
/*气阀的进出状态*/
#define STATUS_IN   "IN"
#define STATUS_OUT  "OUT"

#endif // PUBLICDATA_H
