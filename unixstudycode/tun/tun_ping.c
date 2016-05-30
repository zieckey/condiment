/**
 *  linux TUN 例子 代码来至
 *  http://hi.baidu.com/zkheartboy/blog/item/e96acf33508e4a40ad4b5f88.html和
 *  http://blog.csdn.net/Z_man/archive/2009/05/26/4216530.aspx
 *  建立一个tun0的虚拟网卡进行通信，程序关闭后将消失。
 *  ping 10.0.0.1
 *  Documentation/networking/tuntap.txt
 *  br_select.c bridge based on select system call.
 *  br_sigio.c  bridge based on async io and SIGIO signal.
 *  http://hi.baidu.com/zkheartboy/blog/item/e96acf33508e4a40ad4b5f88.html
 *  http://blogold.chinaunix.net/u3/114446/showart_2245279.html
 *  http://www.ibm.com/developerworks/cn/linux/l-tuntap/index.html
 */



#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <errno.h>
#include <net/route.h>

/**
 *  激活接口
 */
int interface_up(char *interface_name)
{
    int s;

    if((s = socket(PF_INET,SOCK_STREAM,0)) < 0)
    {
        printf("Error create socket :%m\n", errno);
        return -11;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name,interface_name);

    short flag;
    flag = IFF_UP;
    if(ioctl(s, SIOCGIFFLAGS, &ifr) < 0)
    {
        printf("Error up %s :%m\n",interface_name, errno);
        return -21;
    }

    ifr.ifr_ifru.ifru_flags |= flag;

    if(ioctl(s, SIOCSIFFLAGS, &ifr) < 0)
    {
        printf("Error up %s :%m\n",interface_name, errno);
        return -31;
    }

    return 0;

}

/**
 *  设置接口ip地址
 */
int set_ipaddr(char *interface_name, char *ip)
{
    int s;

    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error up %s :%m\n",interface_name, errno);
        return -41;
    }

    struct ifreq ifr;
    strcpy(ifr.ifr_name, interface_name);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = PF_INET;
    inet_aton(ip, &addr.sin_addr);

    memcpy(&ifr.ifr_ifru.ifru_addr, &addr, sizeof(struct sockaddr_in));

    if(ioctl(s, SIOCSIFADDR, &ifr) < 0)
    {
        printf("Error set %s ip :%m\n",interface_name, errno);
        return -51;
    }

    return 0;
}

/**
 *  创建接口
 */
int tun_create(char *dev, int flags)
{
    struct ifreq ifr;
    int fd, err;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
    {
        printf("Error :%m\n", errno);
        return -61;
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags |= flags;

    if (*dev != '\0')
    {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) 
    {
        printf("Error :%m\n", errno);
        close(fd);
        return -71;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

/**
 *  增加到10.0.0.1的路由
 *  同命令:route add 10.0.0.1 dev tun0
 */
int route_add(char * interface_name)
{
    int skfd;
    struct rtentry rt;

    struct sockaddr_in dst;
    struct sockaddr_in gw;
    struct sockaddr_in genmask;

    memset(&rt, 0, sizeof(rt));

    genmask.sin_addr.s_addr = inet_addr("255.255.255.255");

    bzero(&dst,sizeof(struct sockaddr_in));
    dst.sin_family = PF_INET;
    dst.sin_addr.s_addr = inet_addr("10.0.0.1");

    rt.rt_metric = 0;
    rt.rt_dst = *(struct sockaddr*) &dst;
    rt.rt_genmask = *(struct sockaddr*) &genmask;

    rt.rt_dev = interface_name;
    rt.rt_flags = RTF_UP | RTF_HOST ;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(ioctl(skfd, SIOCADDRT, &rt) < 0) 
    {
        printf("Error route add :%m\n", errno);
        return -81;
    }

    return 0;
}
/*
测试方法：

1 先编译运行此程序，监听数据的读写。

2 开启另外一个终端，执行ping命令,就是本地发一个测试包到10.0.0.1

即可看到收到和接收的数据包IP包了。
*/
int main(int argc, char *argv[])
{
    int tun, ret;
    char tun_name[IFNAMSIZ];
    unsigned char buf[4096];
    unsigned char ip[4];

    tun_name[0] = '\0';
    tun = tun_create(tun_name, IFF_TUN | IFF_NO_PI);
    if (tun < 0) 
    {
        return 10;
    }
    printf("TUN name is %s\n", tun_name);

    //激活虚拟网卡增加到虚拟网卡的路由
    if (interface_up(tun_name) < 0) {
        return 12;
    }
    if (route_add(tun_name) <0) {
        return 20;
    }

    while (1) {

        ret = read(tun, buf, sizeof(buf));
        printf("read %d bytes\n", ret);
        int i;
        for(i=0;i<ret;i++)
        {
          printf("%02x ",buf[i]);
        }
        printf("\n");
        if (ret < 0)
            break;
        memcpy(ip, &buf[12], 4);
        memcpy(&buf[12], &buf[16], 4);
        memcpy(&buf[16], ip, 4);
        buf[20] = 0;
        *((unsigned short*)&buf[22]) += 8;
        ret = write(tun, buf, ret);
        printf("write %d bytes\n", ret);
    }

    return 0;
}
