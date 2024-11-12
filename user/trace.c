#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "Usage: trace mask command [args]\n");
    exit(1);
  }

  int mask = atoi(argv[1]);

  // Gọi trace và kiểm tra lỗi
  if (trace(mask) < 0) {
    fprintf(2, "Error: trace failed\n");
    exit(1);
  }

  // Thực thi lệnh với các tham số
  if (exec(argv[2], &argv[2]) < 0) {
    fprintf(2, "Error: exec failed\n");
    exit(1);
  }

  exit(0);
}
