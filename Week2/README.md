# Bài tập Hệ điều hành Nhúng: U-Boot và Kernel

## Nội dung thực hiện

### U-Boot
- Biên dịch U-Boot thành công (MLO, u-boot.img)
- Tạo phân vùng thẻ nhớ và cài đặt U-Boot cho BBB
- U-Boot khởi động thành công qua UART
- Hiển thị thông tin phần cứng và sử dụng được lệnh U-Boot

### Linux Kernel
- Biên dịch Kernel cho BBB (zImage, dtb)
- Chép kernel và dtb vào thẻ nhớ
- Load kernel và dtb lên RAM, boot bằng lệnh `bootz`
- Kernel khởi động thành công và chờ rootfs
