## 日志

#### 7.22及之前

- 学习MIT6.837课程PPT到Leture3，并合到原来计算机图形学的笔记上
- 完成作业0
- 复习《GAMES101-现代计算机图形学入门-闫令琪》
- 学习Makefile文件的编写
- 安装并配置clash
- 复习线性代数

#### 7.23
- 修复了makefile没有办法自动获取依赖的.hpp文件的bug
  - 主要原因是作业prompt里给的示例代码的.C文件并未修改为.cpp文件
- 配置笔记本window里的vscode用ssh连接虚拟机
  - 下载remote explore和ssh插件
  - 设置虚拟机的网络访问方式为“桥接”
  - 手动设置linux的ip，保证虚拟机不会断网（192.168开头的地址，在Windows通过ipconfig查看）
  - 在vscode添加ssh连接
  - 这样就可以用vscode的界面和terminal使用linux环境，防止了在虚拟机直接使用编辑器的卡顿问题

- 编写.gitignore，保证不会commit多余的文件
- 开始写作业1

#### 7.24
- 完成了作业1
  - 完成所有文件和makefile文件的编写
  - 测试了7个输入文件
    - 对于第七个输入文件出现遮挡关系错误的问题。解决方式：最后把Sphere类的intersect中判断$ t_1 $和$ t_2 $哪个是交点的部分的判断条件由t1<0,t2>0改为了t1<t_min, t2>t_min，t_min<0的情况应该是能看到光线后面的物体的？
    - 对于第四个输入文件，出现了输出的球体的投影不圆，类似椭圆的问题。解决方式：检查发现up向量和direction不垂直。
    - 创建这个笔记

#### 7.25
- 学习PPT到Lecture5
  - 补充笔记
- 完成作业2
  - 按要求完成了所有必须内容
    - 待补充：正确维护transform后法向量
  - 测试所有带图片的样例
    - 样例3依然有问题，调了快两小时也没弄清楚
    - 犯了很多低级错误导致调试时间特别长，非常疲惫

#### 7.26
- 学习PPT到Lecture7
- 修复作业2的bug
  - 维护法向量正确transform
  - 对物体的transform可以转化为ray的transform。但是如果发生了scaling，由于新的normal需要标准化，根据t判断哪个物体在前就会发生错误，所以我在transform之前对t乘以了normal标准化前的向量长度length，在transform判断交点之后，再把t除以length。这样才能正确渲染最后一个样例。
- 搭建了作业3的环境
  - 安装opengl(ubuntu)
    - 使用以下命令来安装Mesa OpenGL库：```sudo apt install libgl1-mesa-dev```
    - 使用以下命令来安装OpenGL的头文件：```sudo apt install mesa-common-dev```
    - 安装FreeGLUT：```sudo apt install freeglut3-dev```
    - Makefile中添加`INCLUDE 	= -I/usr/include`, 该目录下有GL文件夹，里面有需要的头文件; Makefile中添加链接库`LIBS = -lm -lGL -lGLU -lglut -L/usr/lib`
  - 修改了作业3代码的文件夹结构
    - 考虑到之前代码不断被改动，导致前面的作业可能无法重新编译，所以每个作业单独一个文件夹。

#### 7.27
- 完成作业3
  - 按r调用自己写的shade函数，拍照。否则，用openGL绘制，只需要把调用题目给出的函数就可以传递渲染所需的所有信息。
  - 特别注意sphere的绘制，要达到图片效果，又要使用惯用的phi和theta的范围和意义，那么球坐标和我们常常认识的球坐标有关的坐标轴要发生变化。
  - glCanvas类会自动帮忙和opengl交互，只需补充相应的要求的成员函数，glCanvas就会自动调用诸如照相、Paint、修改相机参数等。
  
#### 7.28
- 完成作业4
  - 透明长方形的那几个样例依然有问题，暂时查不出，卡了很久很久。光线应该是正确的，颜色和题目给的示例有偏差。但是其他的折射样例都没问题。
  - 折射率需要特殊处理，如果折射率不是1就把下一次折射进入的材质的折射率写成1，因为无论是射入还是射出，交点的材质都是物体的材质。这个问题调了很久才发现。
  - 仍然不清楚折射（或者反射）的颜色损失应该是多少，暂时是把reflective Color的模长设为了损失率。
  - 命令行参数单独抽出来存储在一个类的静态空间
- 学习到lecture9

#### 7.29
- 写作业5
  - 完成了球体的insertIntoGrid
  - 可视化体素
  - 完成MarchingInfo的编写，用于获得光线传播方向的下一个cell
  - 编写了grid类的initializeRayMarch用于初始化MarchingInfo，获取第一个cell的信息
  - 编写了grid类的intersect并测试确保能渲染出正确的体素可视化图像，能通过第一和第二个场景
  - 注意用体素判断相交的时候要从grid开始判断intersect，而不是从最高级的group。

#### 7.30
- 完成作业5
  - 可视化光线的路径，并可视化每个cell与其相交的图形个数，前者在grid的intersect处添加```void RayTree::AddHitCellFace(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Vec3f normal, Material *m);
  void RayTree::AddEnteredFace(Vec3f a, Vec3f b, Vec3f c, Vec3f d, Vec3f normal, Material *m);```后者调用Hit的Set函数修改material未对应频率代表的颜色
  - 完成三角形的体素化代码编写，通过兔子的场景
  - 修复三角形平行坐标轴时没有厚度的bug，通过场景3
  - 完成transform的体素化代码（相当于累积前面的transform），在三角形和球体的显示调用Object3D::insertIntoGrid方法，把带transform的物体的insertintoGrid委托给这个函数实现（球体直接用整个包围盒）
  - 改进Transform的包围盒的求法，避免课件出现的旋转后的三角形包围盒过大的问题
  - 通过全部测试用例
- 写作业6