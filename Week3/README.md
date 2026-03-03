# Week 3: Root File System (RootFS) cho BeagleBone Black

## Mục tiêu
- Biên dịch thành công RootFS sử dụng BusyBox.
- Tạo RootFS tối thiểu và đưa lên thẻ nhớ.
- Liên kết Kernel với RootFS thông qua tham số `root=` trong bootargs.
- Khởi động thành công và sử dụng được các lệnh cơ bản (`ls`, `cat`, `echo`).

## Nội dung thực hiện
- Build BusyBox bằng cross-compile.
- Tạo cấu trúc thư mục RootFS cơ bản.
- Copy thư viện cần thiết vào RootFS.
- Cấu hình U-Boot bootargs để mount RootFS.
- Boot thành công vào RootFS.

## Kết quả
- Hệ thống boot vào RootFS thành công.
- Thực thi được các lệnh BusyBox trên board.
