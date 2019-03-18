# Wechat Hook TO Restful API

这个项目的目的是要把微信pc端所有hook变成restful api   
完全公开免费!!!   
谢Hezone的教程   
https://study.163.com/course/courseMain.htm?courseId=1006486010   
目前项目代码和结构非常简陋, 我打算功能比较完善时再重构一下

我用的pc微信的版本是2.6.7.40, 如果版本不同的话需要自己找到call的基址, 方法在Hezone的教程里有

### 使用方法
注入Debug文件夹里的WechatDllCpp.dll到微信进程, http服务自动开启
地址的127.0.0.1:2019

1. 发消息api(目前只有这个, 功能会慢慢添加)   
127.0.0.1:2019/send   
```
{
    "wxid" : "微信id"
    "msg"  : "要发的信息"
}
```


