# C/C++ Questions
> C/C++面试题

---

## Q：面向对象的特征

-  抽象

（1）数据抽象（类的属性）

（2）过程抽象（类的方法）

- 封装

（1）封装过程与数据

（2）访问只能通过特定的界面，如私有变量，用get/set方法访问

- 继承

（1）一个新类可以从现有类中派生（鼓励类重用）

（2）<font color=red>为实现多态性做准备</font>

- 多态

（1）方法的重载（静态多态）

（2）函数模板（静态多态）

编译时由编译器决定类型，```  template<class T> ```

（3）动态绑定

（4）方法的重写


## Q：C/C++ 内存区

- **C++：五个区**

（1）堆

（2）栈

（3）自由存储区

（4）全局/静态存储区（for 全局/静态变量）

（5）常量存储区（存放常量）

- **C：四个区**

（1）堆

（2）栈

（3）全局/静态存储区

（4）常量存储区

Notice：常量存储区：不占内存，与代码段在一个区

Notice：静态存储区：在内存空间中

**const 变量在C和C++中存储位置不一样**

（1）在C中：栈

（2）在C++中：常量存储区

## Q：const

- **默认状态下，const对象只在文件内有效**

如果想在多个文件之间共享const对象，必须在变量的定义和声明之前加extern关键字。

- **const 和类型的前后顺序可变**

```
const int i = 0;
int const j = 1;
```

- **对const引用时，引用的类型必须与其所引用的对象一致**

```
const int ci = 1024;
const int &r = ci;
```

- **初始化常量类型的引用时，可以用表达式或变量**

此时，不允许通过常量类型的引用去改变其绑定的值。

```
int i = 24;
const int &r = i;
const int &r2 = r * 2;
```

```
double dval = 3.14;
const int &r = dval;

// 实际上编译器的做法是：
const int temp = dval;
const int &r = temp;
```

- **要存放常量对象的地址，只能用指向常量的指针**

```
const double pi = 3.14;
const double *r = &pi;
```

同时类似的，指向常量的指针允许指向非常量对象。

- **summary**

指向常量的指针，或常量类型的引用都不会改变与他们相关（绑定）的对象的值。

Notice：指针是对象，但引用不是。

- **常量指针(const pointer)**

```
int a = 0;
int *const pa = &a; // 指向变量a的常量指针
const double b = 3.14;
const double *const pb = &b; // 指向双精度常量b的常量指针
```

- **顶层const和底层const（top-level const and low-level const）**

对于指针：顶层const是指指针本身是否是常量，底层const是指指针指向的对象是否是常量。

对于变量：只有顶层const，比如 const int ci = 0;

白话：顶层const更加关注自身，顶级重要！

他们的作用：（参考：https://blog.csdn.net/qq_19528953/article/details/50922303）

为啥非要区分顶层const和底层const呢，根据C++primer的解释，区分后有两个作用。

1. 执行对象拷贝时有限制，常量的底层const不能赋值给非常量的底层const。也就是说，你只要能正确区分顶层const和底层const，你就能避免这样的赋值错误。下面举一个例子：

```
int num_c = 3;
const int *p_c = &num_c;  //p_c为底层const的指针
//int *p_d = p_c;  //错误，不能将底层const指针赋值给非底层const指针
const int *p_d = p_c; //正确，可以将底层const指针复制给底层const指针
```

2. 使用命名的强制类型转换函数const_cast时，需要能够分辨底层const和顶层const，因为const_cast只能改变运算对象的底层const。下面举一个例子：

```
int num_e = 4;
const int *p_e = &num_e;
//*p_e = 5;  //错误，不能改变底层const指针指向的内容
int *p_f = const_cast<int *>(p_e);  //正确，const_cast可以改变运算对象的底层const。但是使用时一定要知道num_e不是const的类型。
*p_f = 5;  //正确，非顶层const指针可以改变指向的内容
cout << num_e;  //输出5
```

- **constexpr和常量表达式**

const expression 是指值不会改变，且在编译过程中就能得到计算结果的表达式。

C++11新规定：允许将变量声明为constexpr类型，以便编译器检查变量的值是否是一个常量表达式。 多用constexpr有好处。

```
constexpr int mf = 20; // 20是常量表达式
constexpr int limit = mf + 1; // mf + 1 是常量表达式
constexpr int sz = size(); // 当 size 为 constexpr 函数时，这行正确
```

- **指针和constexpr**

1. constexpr指针的初始值必须是nullptr，0或者某个存储于固定地址中的对象（一般是定义于函数体外的对象）。
2. constexpr只对指针本身有效，与指针所指向的对象无关。

```
const int *p = nullptr; // p 是一个指向常数类型的指针。
constexpr int *q = nullptr; // q 是一个指向整数的常量指针，constexpr把指针对象设置为顶层const。

```

- **类型别名与const**

1. C++11中可以用 using 代替 typedef

```
using SI = Sales_item;
SI item;
// 等价于 typedef Sales_item SI;
```

2. typedef 与 *

```
typedef char *pstring; // pstring 是 char* 指针
const pstring cstr = 0; // cstr 是 char* 常量指针
char *const dstr = 0; // 该行与上行有一样的效果

const char *estr = 0; // const char* 的指针，指向常量字符
```

## Q：auto 类型说明符

- **auto 类型在初始化时，会忽略掉顶层const**
```
const int ci = 0;
auto b = ci; // b 为 int 类型（省略了顶层const）
const auto cb = ci; // cb 为 const int 类型
auto c = &ci; // c 为指向整形常量的指针（保留了底层const）
```

但是！auto在碰到引用时，初始值中的顶层const属性仍然保留。

```
auto &g = ci; // g 是一个整形常量引用
auto &h = 42; // 报错，不能为非常量引用绑定到字面值（常量表达式）
const auto &j = 42; // 正确
```

一条语句定义多个变量：

```
// & 和 * 从属于某个声明符，auto 指的是基本数据类型
auto k = ci, &l = i; // k 是整形，l 是整形引用。
```


## Q：free 底层实现

## Q：引用的底层实现
