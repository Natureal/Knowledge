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

顶层const是指指针本身是否是常量，底层const是指指针指向的对象是否是常量。

- **constexpr和常量表达式**

const expression 是指值不会改变，且在编译过程中就能得到计算结果的表达式。

C++11新规定：允许将变量声明为constexpr类型，以便编译器检查变量的值是否是一个常量表达式。 多用constexpr有好处。

```
constexpr int mf = 20; // 20是常量表达式
constexpr int limit = mf + 1; // mf + 1 是常量表达式
constexpr int sz = size(); // 当 size 为 constexpr 函数时，这行正确
```
