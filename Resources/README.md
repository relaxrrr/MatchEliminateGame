# Match Eliminate Game

基于 Cocos2d-x 3.17.2 开发的消除类匹配游戏。

## 游戏简介

这是一款卡牌消除游戏，玩家需要通过点击主牌区的卡牌与底牌进行匹配（点数相差1）来消除卡牌。游戏目标是消除所有主牌区的卡牌。

## 游戏规则

- **匹配规则**：点击主牌区的卡牌，如果该卡牌的点数与底牌的点数相差1，则可以消除
- **胜利条件**：消除所有主牌区的卡牌
- **失败条件**：备用牌堆消耗完毕，且主牌区没有与底牌匹配的卡牌
- **撤销功能**：支持撤销上一步操作

## 技术架构

项目采用 **MVC（Model-View-Controller）** 架构设计：

### Model（模型层）
- `GameModel`: 游戏数据模型，管理所有卡牌数据和游戏状态
- `CardModel`: 卡牌数据模型，存储卡牌的花色、点数等信息
- `UndoModel`: 撤销操作数据模型

### View（视图层）
- `GameView`: 游戏主视图
- `CardView`: 卡牌视图
- `PlayFieldView`: 主牌区视图
- `StackView`: 备用牌堆视图
- `BottomCardView`: 底牌视图
- `GameResultView`: 游戏结果弹窗视图

### Controller（控制器层）
- `GameController`: 游戏主控制器，协调整个游戏流程
- `PlayFieldController`: 主牌区控制器，处理卡牌点击和匹配逻辑
- `StackController`: 备用牌堆控制器，管理备用牌堆操作
- `CardController`: 卡牌控制器

### 其他模块
- `UndoManager`: 撤销管理器，实现撤销/重做功能
- `GameModelFromLevelGenerator`: 关卡数据生成器
- `LevelConfigLoader`: 关卡配置加载器

## 项目结构
MatchEliminateGame/
├── Classes/ # 游戏源代码
│ ├── AppDelegate.cpp/h # 应用程序入口
│ ├── configs/ # 配置文件
│ │ ├── loaders/ # 配置加载器
│ │ └── models/ # 配置模型
│ ├── controllers/ # 控制器
│ ├── managers/ # 管理器
│ ├── models/ # 数据模型
│ ├── services/ # 服务层
│ ├── utils/ # 工具类
│ └── views/ # 视图
├── Resources/ # 游戏资源（图片、字体等）
├── cocos2d/ # Cocos2d-x 引擎
├── proj.android/ # Android 项目配置
├── proj.ios_mac/ # iOS/macOS 项目配置
├── proj.win32/ # Windows 项目配置
├── proj.linux/ # Linux 项目配置
└── CMakeLists.txt # CMake 构建配置


## 环境要求

- **Cocos2d-x**: 3.17.2
- **CMake**: 3.6 或更高版本
- **编译器**: 
  - Windows: Visual Studio 2015 或更高版本
  - macOS: Xcode 8.0 或更高版本
  - Linux: GCC 5.0 或更高版本
  - Android: Android NDK r13b 或更高版本

## 编译说明

### Windows

1. 使用 CMake 生成 Visual Studio 项目：
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x642. 打开生成的 `.sln` 文件，在 Visual Studio 中编译运行

### Android

1. 使用 Android Studio 打开 `proj.android` 目录
2. 配置 NDK 路径
3. 编译运行

### iOS/macOS

1. 使用 Xcode 打开 `proj.ios_mac/MatchEliminateGame.xcodeproj`
2. 选择目标平台（iOS 或 macOS）
3. 编译运行

### Linux
sh
mkdir build
cd build
cmake ..
make## 功能特性

- ✅ 卡牌匹配消除机制
- ✅ 撤销/重做功能
- ✅ 关卡系统
- ✅ 游戏状态检测（胜利/失败）
- ✅ MVC 架构设计
- ✅ 跨平台支持（Windows、Android、iOS、macOS、Linux）

## 开发说明

### 添加新关卡

在 `Classes/configs/` 目录下配置关卡数据，使用 `LevelConfigLoader` 加载关卡配置。

### 扩展功能

项目采用模块化设计，可以轻松扩展：
- 添加新的视图：在 `Classes/views/` 目录下创建新的视图类
- 添加新的控制器：在 `Classes/controllers/` 目录下创建新的控制器类
- 添加新的模型：在 `Classes/models/` 目录下创建新的模型类

## 许可证

本项目基于 Cocos2d-x 引擎开发，遵循 Cocos2d-x 的许可证。

## 贡献

欢迎提交 Issue 和 Pull Request！

## 作者

[relax]

## 更新日志

### v1.0.0
- 初始版本发布
- 实现基本的卡牌匹配消除功能
- 实现撤销功能