#include "mycalculatorEngine.h"

mycalculatorEngine::mycalculatorEngine()
{
}

QQueue<QString> mycalculatorEngine::Split(const QString& exp)          //分离前缀
{
    QQueue<QString> ret;
    QString num = "";

    for (int i = 0; i < exp.length(); i++)
    {
        if ((exp[i] == '.') || ((exp[i] >= '0') && (exp[i] <= '9')))    //判断小数点和数字
        {
            num += exp[i];
        }

        else if (exp[i] == '(' || exp[i] == ')' || exp[i] == '*' || exp[i] == '/')
        {
            if (!num.isEmpty())
            {
                ret.enqueue(num);        //将数字入队列
                num.clear();
            }
            ret.enqueue(exp[i]);
        }

        else if (exp[i] == '+' || exp[i] == '-')           // + - 需要特殊处理
        {
            if (i == 0)       //表达式开头,说明是正负号
            {
                num += exp[i];
            }

            else if (exp[i - 1] == '(' || exp[i - 1] == '+' || exp[i - 1] == '-' || exp[i - 1] == '*' || exp[i - 1] == '/')
            {
                num += exp[i];
            }
            else        //否则是加减运算符
            {
                if (!num.isEmpty())
                {
                    ret.enqueue(num);        //将数字入队列
                    num.clear();
                }
                ret.enqueue(exp[i]);
            }
        }
    }

    if (!num.isEmpty())         //遍历完成,判断是否还有数字
    {
        ret.enqueue(num);
        num.clear();
    }
    return ret;
}

QQueue<QString> mycalculatorEngine::Transfer(QQueue<QString>& exp)     //将中缀队列转换为后缀队列
{
    QStack<QString> stack;
    QQueue<QString> ret;
    bool num_ok;
    QString symbol;

    while (!exp.isEmpty())
    {
        symbol = exp.dequeue();   //出队列
        symbol.toDouble(&num_ok);

        if (num_ok == true)          //数字
        {
            stack.push(symbol);
        }

        else if (symbol == "+" || symbol == "-")
        {
            while (!stack.isEmpty() && (stack.top() != "("))
            {
                ret.enqueue(stack.pop());     //取出栈顶运算符并入队列
            }
            stack.push(symbol);
        }

        else if (symbol == "*" || symbol == "/")
        {
            while (!stack.isEmpty() && (stack.top() != "(") && (stack.top() != "+") && (stack.top() != "-"))
            {
                ret.enqueue(stack.pop());     //取出栈顶运算符并入队列
            }
            stack.push(symbol);
        }

        else if (symbol == "(")
        {
            stack.push(symbol);
        }

        else if (symbol == ")")
        {
            while (!stack.isEmpty() && (stack.top() != "("))
            {
                ret.enqueue(stack.pop());     //取出栈顶运算符并入队列
            }
            if (stack.top() == "(")
                stack.pop();
        }
    }

    while (!stack.isEmpty() && (stack.top() != "("))         //遍历完成,判断栈里是否为空
    {
        ret.enqueue(stack.pop());     //取出栈顶运算符并入队列
    }return ret;
}

QString mycalculatorEngine::ValidNum(QString str)
{
    QString num;
    if (str.indexOf(".") == -1) //判断是否小数
        return str;

    while (str.length() > 1)   //避免0被去掉
    {
        num = str.right(1);
        if (num == "." || num == "0")
        {
            str.chop(1);
            if (num == ".")
                return  str;
        }
        else
            return str;
    }
    return str;
}

QString mycalculatorEngine::Calculate(QString& l, QString& op, QString& r)
{
    double left, right, res;
    QString ret = "";
    left = l.toDouble();
    right = r.toDouble();
    if (op == "+")
    {
        res = left + right;
    }

    else if (op == "-")
    {
        res = left - right;
    }

    else if (op == "*")
    {
        res = left * right;
    }

    else if (op == "/")
    {
        if ((right > (-0.000000000000001)) && (right < (0.000000000000001)))   //判断除数为0
            return NULL;
        else
            res = left / right;
    }

    //ret.sprintf("%f", res);
    return QString::number(res, 'f', 6);
}

QString mycalculatorEngine::Calculate(QQueue<QString>& exp)            //将后缀队列计算出结果
{
    QStack<QString> stack;
    QString symbol, L, R, op, ret;
    bool num_ok;

    while (!exp.isEmpty())
    {
        symbol = exp.dequeue();   //出队列
        symbol.toDouble(&num_ok);

        if (num_ok == true)      //数字
        {
            stack.push(symbol);
        }
        else                  //运算符
        {
            if (stack.size() < 2)
                return "Error";

            R = stack.pop();
            L = stack.pop();
            ret = Calculate(L, symbol, R);
            if (ret.isEmpty())
                return ret;

            stack.push(ret);
        }
    }
    if (stack.size() == 1) //遍历完成,结果只有一个
    {
        return ValidNum(stack.pop());
    }
    else
    {
        return "Error";
    }
}

QString mycalculatorEngine::Result(const QString& exp)
{
    QQueue<QString> q = Split(exp); //分离中缀
    q = Transfer(q);                //转换为后缀
    return Calculate(q);          //返回结果
}