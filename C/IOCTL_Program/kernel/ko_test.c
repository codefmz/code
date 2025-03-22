#include <asm/io.h>
#include <linux/device.h>
#include <linux/fs.h> // file_operation is defined in this header
#include <linux/init.h> // included for __init and __exit macros
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/module.h> // included for all kernel modules
// #include <linux/scpi_protocol.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("StayrealS");
MODULE_DESCRIPTION("Driver as a test case");

static int majorNumber;
static struct class* test_module_class = NULL;
static struct device* test_module_device = NULL;
#define DEVICE_NAME "test" // define device name
#define CLASS_NAME "test_module"

// 函数原型
static long test_module_ioctl(struct file*, unsigned int, unsigned long);
static int __init test_init(void);
static void __exit test_exit(void);

/// linux/fs.h中的file_operations结构体列出了所有操作系统允许的对设备文件的操作。在我们的驱动中，需要将其中需要的函数进行实现。下面这个结构体就是向操作系统声明，那些规定好的操作在本模块里是由哪个函数实现的。例如下文就表示unlocked_ioctl是由本模块中的test_module_ioctl()函数实现的。
static const struct file_operations test_module_fo = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = test_module_ioctl,
};

// 本模块ioctl回调函数的实现
static long test_module_ioctl(struct file* file, /* ditto */
    unsigned int cmd, /* number and param for ioctl */
    unsigned long param)
{
    uint32_t ret = 0;
    char cs[100] = { 0 };
    /* ioctl回调函数中一般都使用switch结构来处理不同的输入参数（cmd） */
    switch (cmd) {
    case 0: {
        printk(KERN_INFO "[TestModule:] cmd = %u.\n", cmd);
        break;
    }
    case 1: {
        ret = copy_from_user(cs, (void* __user)(uint64_t)param, sizeof(cs));
        if (ret == 0) {
            printk(KERN_INFO "[TestModule:] cmd = %u. param = %s\n", cmd, cs);
        }
        break;
    }
    default:
        printk(KERN_INFO "[TestModule:] Unknown ioctl cmd!\n");
        return -EINVAL;
    }
    return 0;
}

static int __init test_init(void)
{
    printk(KERN_INFO "[TestModule:] Entering test module. \n");
    /*  在加载本模块时，首先向操作系统注册一个chrdev，也即字节设备，三个参数分别为：
        主设备号（填写0即为等待系统分配），设备名称以及file_operation的结构体。返回值为系统分配的主设备号。*/
    majorNumber = register_chrdev(0, DEVICE_NAME, &test_module_fo);
    if (majorNumber < 0) {
        printk(KERN_INFO "[TestModule:] Failed to register a major number. \n");
        return majorNumber;
    }
    printk(KERN_INFO "[TestModule:] Successful to register a major number %d. \n", majorNumber);

    // 接下来，注册设备类
    test_module_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(test_module_class)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_INFO "[TestModule:] Class device register failed!\n");
        return PTR_ERR(test_module_class);
    }
    printk(KERN_INFO "[TestModule:] Class device register success!\n");

    // 最后，使用device_create函数注册设备驱动
    test_module_device = device_create(test_module_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(test_module_device)) { // Clean up if there is an error
        class_destroy(test_module_class); // Repeated code but the alternative is goto statements
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(test_module_device);
    }
    printk(KERN_INFO "[TestModule:] Test module register successful. \n");
    return 0;
}

static void __exit test_exit(void)
{
    // 退出时，依次清理生成的device, class和chrdev。这样就将系统/dev下的设备文件删除，并自动注销了/proc/devices的设备。
    printk(KERN_INFO "[TestModule:] Start to clean up module.\n");
    device_destroy(test_module_class, MKDEV(majorNumber, 0));
    class_destroy(test_module_class);
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "[TestModule:] Clean up successful. Bye.\n");
}

module_init(test_init);
module_exit(test_exit);