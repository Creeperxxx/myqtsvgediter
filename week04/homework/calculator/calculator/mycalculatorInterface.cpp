#include "mycalculatorInterface.h"

mycalculatorInterface::mycalculatorInterface() : QWidget(NULL, Qt::WindowCloseButtonHint)
{
}

bool  mycalculatorInterface::construct()
{
    const char* butnText[20] =
    {
        "<-","CE",
        "7","8","9","+","(",
        "4","5","6","-",")",
        "1","2","3","*","=",
        "0",    ".","/",
    };

    const int butnPos[20][4] =       //存放 x y w h
    {
      {10,50,90,40},{110,50,140,40},                                                    //<- CE
      {10,100,40,40},{60,100,40,40},{110,100,40,40},{160,100,40,40},{210,100,40,40},    //7 8 9 + （
      {10,150,40,40},{60,150,40,40},{110,150,40,40},{160,150,40,40},{210,150,40,40},    //4 5 6 - ）
      {10,200,40,40},{60,200,40,40},{110,200,40,40},{160,200,40,40},{210,200,40,90},    //1 2 3 * =
      {10,250,90,40},               {110,250,40,40},{160,250,40,40},                    //0   . /
    };

    mline = new QLineEdit(this);
    if (mline == NULL)
        return false;
    mline->resize(240, 30);
    mline->move(10, 10);
    mline->setAlignment(Qt::AlignRight);
    mline->setReadOnly(1);
    QFont font;
    font.setFamily("microsoft yahei");
    font.setWeight(QFont::Normal);
    font.setPixelSize(14);
    mline->setFont(font);        //设置字体
    this->setWindowTitle("计算器");
    for (int i = 0; i < 20; i++)
    {
        mbuton[i] = new  QPushButton(butnText[i], this);
        if (mbuton[i] == NULL)
            return false;

        mbuton[i]->resize(butnPos[i][2], butnPos[i][3]);
        mbuton[i]->move(butnPos[i][0], butnPos[i][1]);

        QObject::connect(mbuton[i], SIGNAL(clicked()), this, SLOT(handler_clicked()));

    }
    return true;
}

mycalculatorInterface* mycalculatorInterface::getInstance()      //二阶构造
{
    mycalculatorInterface* ret = new mycalculatorInterface();
    if (ret == nullptr || !ret->construct())
    {
        delete ret;
        return NULL;
    }
    return ret;
}

int  mycalculatorInterface::LastMatchingBoth(QString& str1, const char* str2)      //反向匹配str1和str2
{
    for (int i = str1.length(); i >= 0; i--)
    {
        for (unsigned int j = 0; j < strlen(str2); j++)
            if (str1[i] == str2[j])
                return i;
    }
    return -1;
}

int  mycalculatorInterface::MatchingBoth(QString& str1, const char* str2)          //匹配str1和str2,判断str1是否有str2的字符
{
    for (int i = 0; i < str1.length(); i++)
    {
        for (unsigned int j = 0; j < strlen(str2); j++)
            if (str1[i] == str2[j])
                return i;
    }
    return -1;
}

void mycalculatorInterface::handler_clicked()      //处理按键消息
{
    static int ClearLine = 0;
    static int bracket_cnt = 0;           //圆括号计数
    QPushButton* btn = dynamic_cast<QPushButton*>(sender()); //获取对象
    QString line = mline->text();
    QString text = btn->text();     //获取消息

    if (ClearLine)
    {
        mline->setText("");
        line.clear();
        ClearLine = 0;
    }if (text >= "0" && text <= "9")    //数字
    {
        QString tmp = line.right(1);
        if (tmp.length() && tmp[0] == ')')   //数字前面不能为右括号
        {
            return;
        }
        line += text;
    }

    else if (text == ".")    //小数点
    {
        QString tmp = line.right(1);
        if (tmp.length()) //小数点前面只能是数字
        {
            if (MatchingBoth(tmp, "0123456789") == -1)  //没找到数字
            {
                return;
            }
        }
        else             //小数点前面为空
        {
            return;
        }

        int pos = LastMatchingBoth(line, "+-*/.()");   //反向查找
        if (pos != -1 && line[pos] == '.')        //一串数字只能有一个小数点
        {
            return;
        }
        line += text;
    }

    else if (text == "+" || text == "-")       //加减号
    {
        QString tmp = line.right(1);
        if (tmp.length() && tmp[0] == '.')     //前面不能为:小数点
        {
            return;
        }
        tmp = line.right(2);
        if (tmp.length() == 2)          //前面不能连续有两次加减乘除
        {
            if (tmp[0] == '+' || tmp[0] == '-' || tmp[0] == '*' || tmp[0] == '/' || tmp[0] == '(')
                if (tmp[1] == '+' || tmp[1] == '-' || tmp[1] == '*' || tmp[1] == '/')
                    return;
        }
        line += text;
    }

    else if (text == "*" || text == "/")       //乘除号
    {
        QString tmp = line.right(1);
        if (tmp.length())       //前面不能为:左括号,小数点,加减乘除,
        {
            if (MatchingBoth(tmp, "(.+-*/") != -1) //查找左括号,小数点,加减乘除
            {
                return;
            }
        }
        else                   //乘除号前面不能为空
            return;

        line += text;
    }

    else if (text == "(")       //左括号
    {
        QString tmp = line.right(1);
        if (tmp.length())             //前面不能为:右括号,数字,小数点
        {
            if (MatchingBoth(tmp, ")0123456789.") != -1) //查找右括号,数字,小数点
            {
                return;
            }
        }

        tmp = line.right(2);
        if (tmp.length() == 2)          //前面不能连续有两次加减乘除
        {
            if (tmp[0] == '+' || tmp[0] == '-' || tmp[0] == '*' || tmp[0] == '/')
                if (tmp[1] == '+' || tmp[1] == '-' || tmp[1] == '*' || tmp[1] == '/')
                    return;
        }
        line += text;
        bracket_cnt++;
    }

    else if (text == ")")       //右括号
    {
        QString tmp = line.right(1);
        if (bracket_cnt == 0)  //前面没有左括号
            return;

        if (tmp.length())             //前面不能为:加减乘除,小数点,左括号
        {
            if (MatchingBoth(tmp, "+-*/.(") != -1) //查找加减乘除,小数点,左括号
            {
                return;
            }
        }
        else                    //右括号前面不能为空
            return;

        line += text;
        bracket_cnt--;
    }

    else if (text == "<-")       //<-
    {
        if (line.length())
            line.chop(1);
    }

    else if (text == "CE")       //清空
    {
        line.clear();
        bracket_cnt = 0;
    }

    else if (text == "=" && line.length())
    {
        QString ret = mDec.Result(line);
        if (ret.isEmpty())   //除数为0
        {
            line += " : ";
            line += "除数不能为0";
        }
        else if (ret == "Error")
        {
            line += ":";
            line += "格式出错";
        }
        else
        {
            line += " = ";
            line += ret;
        }
        ClearLine = 1;
    }
    mline->setText(line);
}

void mycalculatorInterface::show()              //显示窗口
{
    QWidget::show();
    this->setFixedSize(this->width(), this->height());
}