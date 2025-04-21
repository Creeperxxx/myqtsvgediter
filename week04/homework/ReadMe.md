
---

# MyCalculator

## 项目概述

MyCalculator 是一个基于 Qt 框架开发的简单计算器应用。它实现了基本的数学运算功能，并通过直观的用户界面提供给用户使用。

## 代码结构与功能实现

### mycalculatorInterface 类

`mycalculatorInterface` 类主要负责创建用户界面及处理用户的交互事件。

- **构造函数 (`mycalculatorInterface::mycalculatorInterface`)**：初始化QWidget，并设置窗口样式。
- **`construct()` 方法**：初始化所有按钮的位置、大小及其文本内容，并连接按钮点击信号到槽函数`handler_clicked()`。此外，还设置了显示表达式的QLineEdit控件。
- **`getInstance()` 方法**：采用二阶构造模式，确保对象成功创建并初始化后才返回实例。
- **`handler_clicked()` 方法**：处理用户点击按钮的动作。根据不同的按钮（数字、小数点、运算符、括号等），进行相应的逻辑处理，如更新当前表达式或执行计算。
- **`show()` 方法**：显示主窗口，并固定其大小以防调整尺寸。

### mycalculatorEngine 类

`mycalculatorEngine` 类承担了实际的计算任务，包括中缀表达式的解析、转换为后缀表达式以及最终计算结果。

- **`Split(const QString& exp)` 方法**：将输入的中缀表达式分割成一系列可以进一步处理的元素（数字、操作符）。此方法能够识别连续的数字（包括小数）作为一个整体，并正确处理正负号。
- **`Transfer(QQueue<QString>& exp)` 方法**：将分割后的中缀表达式队列转换为适合求值的后缀表达式队列。利用栈来临时存储操作符，并根据运算符优先级进行适当的出栈入栈操作。
- **`Calculate(QString& l, QString& op, QString& r)` 方法**：对两个数值执行指定的运算（加、减、乘、除），并处理除以零的情况。
- **`Calculate(QQueue<QString>& exp)` 方法**：对转换后的后缀表达式队列进行求值，通过栈机制来保存中间结果，直到得到最终的计算结果。
- **`ValidNum(QString str)` 方法**：用于去除浮点数末尾不必要的零，保证输出结果的整洁性。
- **`Result(const QString& exp)` 方法**：综合上述过程，从接收原始表达式开始，经过分离、转换到最后的计算，返回最终结果。

## 进一步扩展

本项目的架构设计便于扩展，例如：
- 在 `mycalculatorEngine` 中增加新的数学运算函数。
- 改进现有的UI设计，提升用户体验。
- 优化现有算法，提高计算效率或准确性。

