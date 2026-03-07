# Buildroot on BeagleBone Black

## Bài tập 01: Build OS cho BBB

* Sử dụng Buildroot để build hệ điều hành Linux cho BeagleBone Black.
* Kết quả tạo ra **image hệ thống + toolchain**.
* Flash image vào thẻ SD và boot thành công trên BBB.

## Bài tập 02: Sử dụng Toolchain từ Buildroot

* Viết chương trình **Hello World** bằng C.
* Dùng toolchain do Buildroot tạo để **cross compile trên PC**.
* Copy chương trình vào **RootFS của BBB** và chạy thành công.

Ví dụ:

```
hello
Hello from BBB!
```

## Bài tập 03: Thêm Package vào Buildroot

* Tạo **custom package helloworld** trong thư mục `package/helloworld`.
* Thêm các file `Config.in`, `helloworld.mk` và source `hello.c`.
* Enable package trong `menuconfig` và build lại hệ thống.
* Sau khi boot BBB, chương trình đã được **cài sẵn trong OS**.

Ví dụ:

```
hello
Hello from Buildroot Package!
```

## Kết quả

* Build thành công hệ điều hành Buildroot cho BBB
* Chạy được chương trình cross-compiled
* Tạo và tích hợp thành công **custom package vào Buildroot**
