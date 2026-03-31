#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h> // Cho hàm copy_to_user, copy_from_user
#include <asm/io.h>        // Cho hàm ioremap, iowrite32, ioread32

// --- THÔNG SỐ HARDWARE BEAGLEBONE BLACK (AM335x) ---
#define GPIO1_BASE        0x4804C000  // Địa chỉ vật lý gốc của bộ GPIO1
#define GPIO_SIZE         0x1000      // Kích thước vùng nhớ
#define GPIO_OE           0x134       // Thanh ghi Output Enable (Cấu hình IN/OUT)
#define GPIO_CLEARDATAOUT 0x190       // Thanh ghi Tắt pin
#define GPIO_SETDATAOUT   0x194       // Thanh ghi Bật pin
#define GPIO_DATAOUT      0x13C       // Thanh ghi Đọc trạng thái pin
#define PIN_28            (1 << 28)   // Bit thứ 28 (Tương ứng GPIO 60)

// Biến toàn cục quản lý Device
dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;
static struct device *my_device;
void __iomem *gpio_base_addr; // Con trỏ ảo sau khi ioremap

// ====================================================================
// YÊU CẦU: Open/Release & Read/Write & copy_from_user/copy_to_user
// ====================================================================

static int my_open(struct inode *inode, struct file *file) {
    pr_info("GPIO Driver: Thiet bi da mo!\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    pr_info("GPIO Driver: Thiet bi da dong!\n");
    return 0;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    char status_str[2];
    uint32_t reg_val = ioread32(gpio_base_addr + GPIO_DATAOUT);
    
    // Yêu cầu: Cung cấp lệnh đọc trạng thái LED
    if (reg_val & PIN_28) status_str[0] = '1';
    else status_str[0] = '0';
    status_str[1] = '\n';

    // Yêu cầu: Sử dụng hàm copy_to_user
    if (copy_to_user(user_buffer, status_str, 2)) return -EFAULT;
    
    return 2;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t size, loff_t *offset) {
    char cmd[2];

    // Yêu cầu: Sử dụng hàm copy_from_user
    if (copy_from_user(cmd, user_buffer, 2)) return -EFAULT;

    // Yêu cầu: Cung cấp lệnh ghi trạng thái LED
    if (cmd[0] == '1') {
        iowrite32(PIN_28, gpio_base_addr + GPIO_SETDATAOUT); // ON
    } else if (cmd[0] == '0') {
        iowrite32(PIN_28, gpio_base_addr + GPIO_CLEARDATAOUT); // OFF
    }
    return size;
}

// ====================================================================
// YÊU CẦU: Đăng ký thông qua static struct file_operations
// ====================================================================
static struct file_operations fops = {
    .owner   = THIS_MODULE,
    .open    = my_open,
    .release = my_release,
    .read    = my_read,
    .write   = my_write,
};

// ====================================================================
// YÊU CẦU: Hoàn thiện Init/Exit & Cấp phát tự động & ioremap
// ====================================================================

static int __init my_driver_init(void) {
    uint32_t reg_val;

    // Yêu cầu: Tự động cấp phát Major/Minor
    alloc_chrdev_region(&dev_num, 0, 1, "my_gpio_device");
    cdev_init(&my_cdev, &fops);
    cdev_add(&my_cdev, dev_num, 1);

    // Yêu cầu: Tự động tạo file Device trong thư mục /dev/ (Tạo Class, Tạo Device)
    my_class = class_create(THIS_MODULE, "my_gpio_class");
    // Tên file device sẽ là: /dev/my_gpio_led
    my_device = device_create(my_class, NULL, dev_num, NULL, "my_gpio_led"); 

    // Yêu cầu: Sử dụng ioremap để truy cập bộ nhớ vật lý
    gpio_base_addr = ioremap(GPIO1_BASE, GPIO_SIZE);
    
    // Yêu cầu: Cấu hình chân LED ở chế độ phù hợp (Output)
    reg_val = ioread32(gpio_base_addr + GPIO_OE);
    reg_val &= ~PIN_28; // Ghi bit 0 vào thanh ghi OE để làm Output
    iowrite32(reg_val, gpio_base_addr + GPIO_OE);

    pr_info("GPIO Driver: Khoi tao thanh cong! Device: /dev/my_gpio_led\n");
    return 0;
}

static void __exit my_driver_exit(void) {
    iowrite32(PIN_28, gpio_base_addr + GPIO_CLEARDATAOUT); // Tắt LED trước khi thoát
    iounmap(gpio_base_addr); // Giải phóng ioremap

    // Gỡ bỏ cấu trúc /dev/
    device_destroy(my_class, dev_num);
    class_destroy(my_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    
    pr_info("GPIO Driver: Go bo thanh cong!\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SinhVien");
