#include <iostream>
#include <sys/sysctl.h>

int main() {
	// int i, mib[4];
	// size_t len;
	// struct kinfo_proc kp;

	// /* Fill out the first three components of the mib */
	// len = 4;
	// sysctlnametomib("kern.proc.pid", mib, &len);

	// /* Fetch and print entries for pid's < 100 */
	// for (i = 0; i < 100; i++) {
	// 		mib[3] = i;
	// 		len = sizeof(kp);
	// 		if (sysctl(mib, 4, &kp, &len, NULL, 0) == -1)
	// 				perror("sysctl");
	// 		else if (len > 0)
	// 				printkproc(&kp);
	// }
	auto a = CTL_HW;

}
// hw.l1dcachesize
// hw.l2cachesize