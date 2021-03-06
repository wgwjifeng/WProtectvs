// VMPackLib


#ifndef __CVIRTUALMEMORY__
#define __CVIRTUALMEMORY__


// 虚拟地址内存类
class CVirtualMemory
{

public:
	// 虚拟地址所对应的内存.
	char* m_BaseAddr;
	// 内存的空间大小
	int m_addrlen;
	//虚拟内存地址
	DWORD m_VirtualBaseAddress;
private:
	// 还未使用的起始地址
	char* CurrentAddress;
public:

	CVirtualMemory();

	CVirtualMemory(DWORD VirtualAddress,
			   int len);

	~CVirtualMemory();

	// 创建一片对应虚拟地址的内存空间
	void	CreateVirtualMemory(DWORD VirtualAddress,
						  int len);

	// 根据内存地址换算出虚拟线性地址
	DWORD	GetVirtualAddress(char* addr);

	// 根据虚拟地址计算出内存空间地址
	char*	GetAddrOfVirtualAddr(DWORD VirtualAddress);

	// 复制数据到虚拟内存,返回数据的起始虚拟地址.
	DWORD	WriteData(char* src,int len);
	//复制数据到指令虚拟内存地址
	DWORD	WriteData(DWORD VirtualCode,char* src,int len);
	// 清空内存
	void	ClearMemory();

	// 获得当前可用空间
	char*	GetCurrentAddr(void);
	// 获得当前可用的虚拟地质
	DWORD	GetCurrentVirtualAddress(void);
};// END CLASS DEFINITION CVirtualMemory

#endif // __CVIRTUALMEMORY__
