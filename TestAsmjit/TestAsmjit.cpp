// TestAsmjit.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AsmJit.h"
using namespace asmjit;

typedef int(*FUNC)();

void MinimalExample()
{
	JitRuntime rt;
	CodeHolder code;  //�洢������ض�λ��Ϣ
	code.init(rt.getCodeInfo());//

	X86Assembler assm(&code); //���������������code
	assm.mov(x86::eax, 1); //mov eax, 1
	assm.ret(); //�Ӻ�������
	// **** X86Assember ������Ҫ�����Ա����ٺ��ͷ� 

	FUNC pfn = 0;
	Error err = rt.add(&pfn, &code); //�������Ĵ������ӵ�runtime
	if (err)
	{
		return; //����
	}
	// *** CodeHoder ������Ҫ�����Ա�����

	int result = pfn(); //ִ�д���
	printf("%d\r\n", result);

	rt.release(pfn);
}

/*
* ������
*/
void UsingOpransExample(X86Assembler& assm)
{
	// ����������
	X86Gp dst = x86::ecx;
	X86Gp src = x86::rax;
	X86Gp idx = x86::gpq(10); //��ȡr10
	X86Mem m = x86::ptr(src, idx); //�����ڴ��ַ[src+idx] - ����[rax+r10]

	//���� m
	m.getIndexType(); //����X86Reg::kRegGpq
	m.getIndexId(); //���� 10 ��r10��

	//���¹���mem�е�idx
	X86Gp idx_2 = X86Gp::fromTypeAndId(m.getIndexType(), m.getIndexId());
	idx == idx_2; //true, ��ȫ��ͬ

	Operand op = m;
	op.isMem(); //True,����ǿת��mem��x86mem

	m == op; //true, op ��mm�ݿ���
	static_cast<Mem&>(op).addOffset(1); //��ȫ��Ч��
	m == op; //False, opָ��[rax+r10+1], ������[rax+r10]

	//����mov
	assm.mov(dst, m);//���Ͱ�ȫ
	//assm.mov(dst, op); //û���������

	assm.emit(X86Inst::kIdMov, dst, m); //����ʹ�ã��������Ͳ���ȫ
	assm.emit(X86Inst::kIdMov, dst, op);//Ҳ���ԣ�emit�����Ͳ���ȫ�ģ����ҿ��Զ�̬ʹ��
}

typedef int(*PFN_SUM)(int* arr, int nCount);

void AssemblerExample()
{

	JitRuntime rt;
	CodeHolder code;  //�洢������ض�λ��Ϣ
	code.init(rt.getCodeInfo());//

	X86Assembler a(&code); //���������������code

	X86Gp arr, cnt;
	X86Gp sum = x86::eax; //eax�����ֵ

	arr = x86::edx; //��������ָ��
	cnt = x86::ecx; //���������

	a.push(x86::ecx);
	a.push(x86::edx);

	a.mov(arr, x86::ptr(x86::esp, 12)); //ȡ��һ������
	a.mov(cnt, x86::ptr(x86::esp, 16)); //ȥ�ڶ�������

	Label lblLoop = a.newLabel(); //Ϊ�˹���ѭ����������Ҫ��ʶ��
	Label lblExit = a.newLabel();

	a.xor_(sum, sum); //����sum�Ĵ���
	a.test(cnt, cnt); //�߽���
	a.jz(lblExit); // if cnt == 0 jmp Exit
	
	a.bind(lblLoop); //ѭ����������ʼ
	a.add(sum, x86::dword_ptr(arr)); //sum += [arr]
	a.add(arr, 4); //arr++
	a.dec(cnt); //cnt --
	a.jnz(lblLoop);// if cnt != 0 jmp Loop

	a.bind(lblExit); //�˳��߽�
	a.pop(x86::edx);
	a.pop(x86::ecx);
	a.ret();//���� sum(eax)


	PFN_SUM pfnSum = 0;
	Error err = rt.add(&pfnSum, &code); //�������Ĵ������ӵ�runtime
	if (err)
	{
		return; //����
	}
	// *** CodeHoder ������Ҫ�����Ա�����
	int anArr[10] = {1,2,3,4,5,6,7,8,9,10};
	int result = pfnSum(anArr, 10); //ִ�д���
	printf("%d\r\n", result);
}
int _tmain(int argc, _TCHAR* argv[])
{
	//MinimalExample();
	AssemblerExample();

	JitRuntime rt;
	CodeHolder code;  //�洢������ض�λ��Ϣ
	code.init(rt.getCodeInfo());//

	X86Assembler assm(&code); //���������������code
	UsingOpransExample(assm);

	return 0;
}
