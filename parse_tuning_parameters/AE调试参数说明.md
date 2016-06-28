>作者: 郑子翔(zzx@meizu.com)

##AE调试参数说明

AE tuning 参数调整主要包括如下的五个部分。

-   [1 metering_config](#megering_config)
    -   [1.1 center metering](#center_metering)
    -   [1.2 spot metering](#spot_metering)
    -   [1.3 average metering](#average_metering)

-   [2 base_config](#base_config)
    -   [2.1 target brightness](#target_brightness)
    -   [2.2 stable brightness](#stable_brightness)
    -   [2.3 brightness limit](#brightness_limit)
    -   [2.4 gain limit](#gain_limit)
    -   [2.5 default exposure](#default_exposure)
    -   [2.6 minimal exposure time](#minimal_exposure_time)

-    [3 convergent_table](#convergent_table)
    -   [3.1 target brightness value](#target_brightness_value)
    -   [3.2 convergent table 1](#convergent_table_1)
    -   [3.3 convergent table 2](#convergent_table_2)
    -   [3.4 shuter-gain compensation](#shutter-gain-compensation)

-    [4 environment_info](#environment_info)

-    [5 dynamic_compensation](#dynamic_compensation)

## megering_config

主要是根据不同的测光方式，改变对3AA的统计信息的获取，以及计算的配置项，它会影响到常见的测光方式的权重设置。

### center_metering

中心权重测光，centerWidth, centerHeight, centerMap　对应的是中心权重测光下，对应统计信息块的宽和高，以及每个单位上对应的权重大小。

在Exynos 7420上，32 是目前能够设置的最大的值，因为目前三星的3AA 统计信息的输出的Y, R, G, B的平均值的最大的二维矩阵就是32 x 32大小。即，我们在中心权重测光利用了所有曝光区域的有效采样。



### spot_metering

点测光，soptWidth, spotHeight, spotMap 对应的是点测光下，对应的统计信息块的宽和高，以及每个单位上对应的权重大小。

需要注意的是，点测光的时候，我们不需要采样所有的曝光区域，而是用户点击的坐标的周围的部分曝光区域。目前给定的是以用户指定的区域为中心的9x9的方格，权重设定如spotMap的数组所示。

eg: spotMap:
"spotMap" : [
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8,
]


### average_metering

平均测光，是指采样所有曝光区域，以及每个单位区域内的曝光权重一致。故，目前算法接口提供该模式的支持，但是没有提供调试设定参数。



注意：无论上面的那种测光方式，测光区域始终不能大于该ISP 芯片所能支持的最大的曝光区域划分的大小，在Exynos7420 & Exynos5430 上，该大小都是32x32.


## base_config

主要记录了测光中基本信息.

### target_brightness
目标亮度表示曝光平衡的条件是收敛到目标亮度范围.
Exynos7420中对Y的强度表示的带宽是14bit, 即最大值是4096. 在18%灰卡，以及2.2的gamma 设定下，获取最佳的曝光照片，经过分析，给定的目标亮度，targetBr约600左右，故，给定的目标亮度范围为:(575,625).
该范围不易过大，容易导致，亮度收敛不一的现象，也不能过小，会导致收敛的过程中在接近曝光平衡时，出现轻微闪烁(该震荡根据收敛的幅度很有可能不断持续).

### stable_brightness
稳定亮度表示当前AE 收敛到了目标亮度附近.
当AE 收敛到目标亮度之后，我们认为曝光平衡，而，需要再次触发重新测光的条件是，当前的统计信息Y Avg算出的来的亮度值要超出稳定亮度的范围. 这个有利于AE的稳定，不会因为很小的亮度差异，就导致再次的收敛.
目前的稳定亮度给定为(550, 650),该区间不易过大.

### brightness_limit
该值由平台来定，一般定义为平台AE stat bandwith的最大值附近.

###gain_limit
限定当前平台的模拟增益(expTimeDefault)，以及数字增益(expTimeDefault)的最大值，根据Sensor,以及ISP 来确定其大小

###default_exposure
相机启动时，设定的默认曝光时间(expTimeDefault, 单位us)和增益(expTimeDefault,单位为x1).

###minimal_exposure_time
最小曝光行时间，由sensor spec 决定，它也决定了AE允许的最小快门时间.


## convergent_table
表示AE在不同条件下的收敛幅度.

### target_brightness_value
该值为确定的targetBrMin,targetBrMax的中值.这里用于区分收敛时候，是过曝，还是曝光不足.

### convergent_table_1
在曝光不足的情况下(当前亮度小于target birghtness value)，当前亮度和收敛幅度的调整.
收敛幅度是相对上一帧曝光的总的曝光量进行调整.
tableCount1 表示曝光不足下的收敛表的长度.
table1 表示收敛查询表.

### convergent_table_2
在过曝光的情况下(当前亮度大于target birghtness value)，当前亮度和收敛幅度的调整.
收敛幅度是相对上一帧曝光的总的曝光量进行调整.
tableCount2 表示过曝的收敛表的长度.
table2 表示收敛查询表.

### shutter-gain-compensation
快门时间和增益的修正.主要用于在室内交流电灯光下，避免出现在不恰当的曝光下，引起的水波纹现象(anti-flicker).

主要分为自动，50hz, 60hz, 在自动下，我们默认用的50 hz的shutter-gain　修正表.

## environment_info
用于判定当前环境亮度的强弱.
目前分为室内，室外，鉴于室内室外的中间，低光，高光以及整体环境亮度检测.

该计算方法是根据具体模组而定，根据具体模组在单位快门时间，单位增益下，确定环境亮度强度.

eg:
"indoorBvRange" : [70000.0, 32.0, 600.0, 30000.0, 1.0, 600.0],

我们认为，在70ms的快门时间下，32倍增益放大下，获取到的照片的亮度在600的时候为室内的极小值.
在30ms的快门时间下，1倍增益放大下，获取到的亮度在600的时候为室内的极大值.


## dynamic_compensation
动态修正是动态AE 调整的策略配置选项.主要有，时能开关，纯白检测阀值，纯白亮度补偿强度，低光亮度补偿强度
