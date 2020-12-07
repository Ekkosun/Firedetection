# 程序设计 

------

**设计要求：统一使用驼峰式命名法**

## UI

#### 设计：

* 开始按钮

* 采用`slider`实现检测阈值和告警阈值的设置
  * `slider`范围为1~100，单位为百分制
  * 并将当前阈值展示出来
* 采用两个`radio button`来选择检测方式

#### 待解决的问题

* 将窗口改进成可以随着屏幕分辨率不同进行缩放的
* 视频在界面中的显示



#### Cmake问题

* 生成的`exe`不能运行，要使用`windeployqt`进行打包才能运行。



## 检测

#### 设计 ：

* 封装成类

```c++
//获取图片流的类，imageStreamObtain.h
class imageStreamObtain{
    imageStreamObtain()
}
```



```c++
//检测类，供继承。detect.h
class detect{
    public:
    	void setSensitivity(double);  //设置灵敏度
        virtual bool detect(Mat imageIn); 	//检测接口
    
    private: 
    	double sensitivity;   //检测的敏感度：OpenCV使用sensitivity调整R分量的放缩、目标检测中当作threhold
    	bool ifDetected;	  //输出的检测结果
}
```

```c++
//opencv检测，继承detect类，opencvDetect.h
#include <detect.h>
class opencvDetect: public detect{
    
}
```



