// sysinfo.h
#ifndef SYSINFO_H
#define SYSINFO_H

// Cấu trúc thông tin hệ thống
struct sysinfo
{
    uint64 freemem; // Lượng bộ nhớ còn trống (bytes)
    uint64 nproc;   // Số lượng tiến trình đang chạy
};

#endif // SYSINFO_H
