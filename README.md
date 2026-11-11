# Zippy

UE 5.0 C++ 练习项目：扩展 `CharacterMovementComponent`，实现可联网预测的疾跑、滑铲、匍匐。

## 实现内容

- **疾跑**：`Safe_bWantsToSprint` 写入 `FSavedMove` 的 `FLAG_Custom_0`，客户端预测、服务器按同一标志位重放。
- **滑铲**：自定义移动模式 `CMOVE_Slide` + `PhysSlide`，速度高于阈值且有可滑地面时进入，带进入冲量、重力分量与摩擦。
- **匍匐**：自定义移动模式 `CMOVE_Prone` + `PhysProne`，长按蹲 0.2 s 进入（蹲伏或滑铲中），独立的最大速度与制动减速。
- **网络预测**：自定义 `FSavedMove_Zippy`（`CanCombineWith` / `GetCompressedFlags` / `SetMoveFor` / `PrepMoveFor`）与 `FNetworkPredictionData_Client_Zippy`，服务器在 `UpdateFromCompressedFlags` 中还原输入。

核心代码：[`ZippyCharacterMovementComponent.h`](Source/Zippy/Public/ZippyCharacterMovementComponent.h) / [`.cpp`](Source/Zippy/Private/ZippyCharacterMovementComponent.cpp)

## 打开工程

仓库未包含模板素材 `Content/StarterContent` 与 `Content/Characters`（体积约 1 GB）。从 UE 5.0 第三人称模板（含 Starter Content）新建一个工程，把这两个文件夹复制到本项目 `Content/` 下即可。
