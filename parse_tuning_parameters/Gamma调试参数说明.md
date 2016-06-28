>作者: 郑子翔(zzx@meizu.com)

##Gamma调试参数说明

Gamma tuning 参数调整主要包括如下的三个部分。

-   [1 base_config](#base_config)
    -   [1.1 normal gamma settings](#normal-gamma)
    -   [1.2 indoor gamma settings](#indoor-gamma)
    -   [1.3 outdoor gamma settings](#outdoor-gamma)
    -   [1.4 gamma-x settings](#gamma-x)
    -   [1.5 s-gamma settings](#s-gamma)
-   [2 compensation](#compensation)

-   [3 filter_table](#filter_table)

## base_config 

对Gamma进行常规参数设置.
在Exynos7420上，进行Gamma矫正的数据强度是16bit，即最大是16384.
根据Gamma 变化的基本原理，我们扩展了低部以及高亮修正，以及s型Gamma变换.

### normal-gamma
normal gamma 即标准gamma, 根据sRGB以及sensor 的感光能力，我们给定的是2.2.
同时，为了抑制底部细节，我们将整个2.2 Gamma曲线向左移动128,　即-128.

### indoor-gamma
在室内，由于低光的感光性性能偏弱，会导致整体的动态范围变窄，在AE 进行低光强度补偿的同时，我们降低底部Gamma 为2.0，避免暗部偏灰。

### outdoor-gamma
在室外，Gamma 加强，凸显底部细节，同时，底部offset 为-128,避免纯黑物体发灰，同时，高亮offset 降低，避免高亮大面积过曝.

### gamma-x
Gamma-x　是确定ISP Gamma x index 的表，Gamma x 范围是0-16384,但是硬件的输入是32 个数值，同时，Gamma 算法需要确认Gamma 变换后的32 个点，这样，Gamma-x 以及变换后的Gamma-y的32个数值对用于模拟一条Gamma曲线，设定到ISP.

Exynos7420的Gamma mapping的动作是在RGB 色彩空间，目前，经过软件修改，可单独设定R,G,B三个通道的Gamma.　默认的，RGB 三通道的Gamma值是一样的.

### s-gamma
S-Gamma 是对目标Gamma 曲线做一次s 型矫正，该矫正的目的是对暗部细节偏灰的现象做抑制，并增加中低部的对比度。

针对Gamma 基础设定以及s-gamma,可参考gamma_tuning.apk(https://github.com/lovelers/gamma_tuning).

##compensation
针对Gamma 进行动态补偿，drcEnable是动态补偿的时能开关.
drcStrength 动态补偿强度.

##filter_table
该结构用于扩展滤镜算法的接口.
包含rgbFlag，以及rgb 的数组结构.

rgbFlag 标示是rgb数组结构中R, G, B三个通道分别的数据类型.
其中，
* 0表示不使用rgb 数组数据，利用算法输出结果.
* 1表示针对计算出来的Gamma结果的每个点进行求反，即gamma_y = 16384 - gamma_x.
* 2表示针对计算出来的Gamma结果进行乘积变换，即gamma_y = gamma_y * value / 10000.
* 3表示用rgb数组中的值代替算法输出结果.

eg:
```
"rgbFlag" : [3, 3, 3],
"rgb" : [
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384,
    16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384,

    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384,
    16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384,

    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384,
    16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384,
]
```
表示r, g, b 通道的gamma 结果都要用rgb数组中的数据替换.
其表示的滤镜效果就是黑白.

