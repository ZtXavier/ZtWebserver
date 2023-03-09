# ZWebServer


## 项目介绍:
- 该项目是由C++11编写的Web服务器，其功能实现了解析get、head请求，可处理静态资源，支持HTTP长连接，支持管线化请求，并实现了异步日志，记录服务器运行状态。


## 项目构建

``./build.sh``

## 启动命令

``../build/ZWebServer/ZWebServer [-t threadnums] [-p port] [-l logfilepath]``



## 技术特点:

- 使用Epoll的ET非阻塞模型,IO多路复用技术,采用Reactor模型框架
- 使用多线程充分利用CPU的性能,使用线程池来维护线程的创建与销毁,来避免线程频繁的创建带来开销
- 在Reactor框架下采用主线程来Accept连接请求,并通过Round Robin的方式分发给其他的IO线程,并采用锁的机制来同步线程.
- 使用双缓冲技术实现简单的异步日志系统
- 使用智能指针技术和RAII机制来减少内存泄漏
- 使用状态机来一步步解析HTTP请求,支持管线化
- 使用eventfd实现线程的异步唤醒
- 使用基于最小根堆的定时器关闭超时请求
- 支持优雅关闭连接

## 并发模型

- 该项目的主要模型角色构成是由一个MainReactor和多个SubReactor来完成需求任务.其中MainReactor来负责响应Client请求,并建立连接,建立连接后通过Round Robin的方式分配给某个SubReactor,其中涉及到多线程对任务的争抢,需要通过加锁来实现同步,锁的创建由线程中的loop完成,所以会在主线程和子线程之间竞争.

- SubReacto可以有一个或多个,会在一个独立的线程中运行,并且会维护一个独立的NIO Selector.

- 如果主线程将新的连接分配给了某个SubReactor,该线程可能正在阻塞在epoll_wait,这里运用到eventfd函数来进行异步唤醒,触发epoll_wait唤醒,得到活跃的事件来处理.

- 该Web服务器整个思路是由事件推动来完成的,所以整个过程中需要注册回调函数,而在RunInloop和queueInLoop两个方法中来执行用户注册的某个回调函数,其中queueInLoop的特点是可以跨进程调用,非常巧妙.


## 定时器

- 定时器的实现是基于priority_queue实现的,底层是小根堆,采用惰性删除的方式,也就是每次时间更新不会唤醒线程,而是在每次循环后进行检查,如果线程很忙,那么检查时间间隔也就很短,反之会很长,但是超时请求并不是该项目的重要痛点.


## 主要部分

- Channel类: 该类会注册一个IO事件,并且属于一个EventLoop,当有IO事件发生时,会调用已经注册好的回调函数处理相应的事件,因此,程序中所有带有读写事件对象都会和一个Channel关联,包括loop中的eventfd等.

- Eventloop类:  One loop per thread是EventLoop类的设计思路,每个线程中只有一个Eventloop对象,每次从poller中拿到活跃的事件,并给到Channel中处理,loop函数会无限循环直到某一轮查询到退出状态后再退出.

## 日志

学习了muduo的日志库,了解到log也分为前端和后端,而写的过程必定涉及到IO以及磁盘的写入,如果不做处理速度肯定是很慢,所以日志有一下特点:

- 为了解决磁盘IO速度慢的缺点,这里设计采用了双缓冲区的思想,但实现采用四个缓冲区,每两个为一组,在第一个缓冲区写满后使用swap来交换两个指针,这样便实现异步写入磁盘.

- fileappend作为最底层的文件类,底层使用操作系统标准IO,直接调用append函数直接向文件写

- loggfile封装了fileappend,并且设置了刷盘的限制count,当达到该count时进行刷盘

- logstream用来格式化输出,重载<< 运算符,内部含有一块缓冲区,将多个<<的结果连成一块

- logging是对外的接口,在该类中定义的logstream对象,这层在输出信息的基础上添加了文件的路径和行数

- AsyncLogging作为异步日志的核心,其作用是负责启动一个log线程,专门用来将log写入logfile,使用双缓冲技术,负责将缓冲区的数据写入logfile中