/*++
*************************************************************************************************
*                Copyright (c) 2008~2010 freevanx.  All rights reserved.
*
*                        freevanx@gmail.com
*
*	This file is distributed under BSD liscense
*	
*	File:
*	Description:
*
--************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/nearptr.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/movedata.h>
#include <dpmi.h>
#include <go32.h>


//int __djgpp_map_physical_memory(void *our_addr, unsigned long num_bytes, unsigned long phys_addr);

#define	QIO_PCI_ADDR_PORT	0xCF8
#define	QIO_PCI_DATA_PORT	0xCFC

typedef	unsigned char	UCHAR, UINT8;
typedef	unsigned short	USHORT, UINT16;
typedef	unsigned long	ULONG, UINT32;
typedef	unsigned int	UINT;

#define	__BUILDIO(bwl, bw, type)

static inline void out##bwl(unsigned type value, int port)
{
	out##bwl##_local(value, port);
}

static inline unsigned type in##bwl(int port)
{
	return in##bwl##_local(port);
}

#define	BUILDIO(bwl, bw, type)

static inline void out##bwl##_local(unsigned type value, int port)
{
	asm volatile("out" #bwl " %" #bw "0, %w1" : : "a"(value), "Nd"(port));
}

static inline unsigned type in##bwl##_local(int port)
{
	unsigned type value;
	asm volatile("in" #bwl " %w1, %" #bw "0" : "=a"(value) : "Nd"(port));
	return value;
}

__BUILDIO(bwl, bw, type)

static inline void outs##bwl(int port, const void *addr, unsigned long count)
{
	asm volatile("rep; outs" #bwl : "+S"(addr), "+c"(count) : "d"(port));
}

static inline void ins##bwl(int port, void *addr, unsigned long count)
{
	asm volatile("rep; ins" #bwl : "+D"(addr), "+c"(count) : "d"(port));
}

BUILDIO(b, b, char)
BUILDIO(w, w, short)
BUILDIO(l, , int)


static inline unsigned char readb(unsigned long addr)
{
	unsigned char ret=0;
	unsigned long new_limit = 0xFFFFFFFF;
	unsigned long old_limit = 0;

	old_limit = __dpmi_get_segment_limit(_my_ds ());

	if (__dpmi_set_segment_limit (_my_ds (), new_limit) == 0)
	{
		if (__dpmi_get_segment_limit (_my_ds ()) != new_limit)
		{
			/* The DPMI host ignored the call.  Fail.  */
			printf("readb: Set segment limit error!\n");
			return 0xFF;
		}
		else
		{
			//__dpmi_set_segment_limit (__djgpp_ds_alias, new_limit);
			//__dpmi_set_segment_limit (_my_cs (), new_limit);
			//_crt0_startup_flags |= _CRT0_FLAG_NEARPTR;
			if (__djgpp_nearptr_enable())
			{
				unsigned char *phy_addr = (unsigned char *)(__djgpp_conventional_base + addr);
				ret = *phy_addr;
				__djgpp_nearptr_disable();
				__dpmi_set_segment_limit (_my_ds (), old_limit);
				return ret;
			}
		}
	}
	printf("readb: unknow error!\n");
	return 0xFF;
}

static inline unsigned char breadb(unsigned long addr)
{
	void * virtualAddress = NULL;
	unsigned long    pageaddr = 0;
	unsigned long    size = 0x1000;
	unsigned long    offset = 0;
	unsigned char value = 0;

	pageaddr = addr & 0xFFFFF000;
	offset = addr & 0x00000FFF;

	if (__djgpp_map_physical_memory(virtualAddress, size, pageaddr) == 0)
	{
		value = *((unsigned char *)((unsigned long)virtualAddress + offset));
		return value;
	}
	return 0xFF;
}

static inline unsigned short readw(unsigned long addr)
{
	unsigned short    ret=0;
	unsigned long new_limit = 0xFFFFFFFF;
	unsigned long old_limit = 0;

	old_limit = __dpmi_get_segment_limit(_my_ds ());

	if (__dpmi_set_segment_limit (_my_ds (), new_limit) == 0)
	{
		if (__dpmi_get_segment_limit (_my_ds ()) != new_limit)
		{
			/* The DPMI host ignored the call.  Fail.  */
			printf("readw: Set segment limit error!\n");
			return 0xFF;
		}
		else
		{
			if (__djgpp_nearptr_enable())
			{
				unsigned short *phy_addr = (unsigned short *)(__djgpp_conventional_base + addr);
				ret = *phy_addr;
				__djgpp_nearptr_disable();
				__dpmi_set_segment_limit (_my_ds (), old_limit);
				return ret;
			}
		}
	}
	printf("unknow error!\n");
	return 0xFF;
}

static inline unsigned short breadw(unsigned long addr)
{
	void * virtualAddress = NULL;
	unsigned long    pageaddr = 0;
	unsigned long    size = 0x1000;
	unsigned long    offset = 0;
	unsigned short value = 0;

	pageaddr = addr & 0xFFFFF000;
	offset = addr & 0x00000FFF;

	if (__djgpp_map_physical_memory(virtualAddress, size, pageaddr) == 0)
	{
		value = *((unsigned short *)((unsigned long)virtualAddress + offset));
		return value;
	}
	return 0xFF;
}

static inline unsigned long readl(unsigned long addr)
{
	unsigned long    ret=0;
	unsigned long new_limit = 0xFFFFFFFF;
	unsigned long old_limit = 0;

	old_limit = __dpmi_get_segment_limit(_my_ds ());

	if (__dpmi_set_segment_limit (_my_ds (), new_limit) == 0)
	{
		if (__dpmi_get_segment_limit (_my_ds ()) != new_limit)
		{
			/* The DPMI host ignored the call.  Fail.  */
			printf("readl: Set segment limit error!\n");
			return 0xFF;
		}
		else
		{
			if (__djgpp_nearptr_enable())
			{
				unsigned long *phy_addr = (unsigned long *)(__djgpp_conventional_base + addr);
				ret = *phy_addr;
				__djgpp_nearptr_disable();
				__dpmi_set_segment_limit (_my_ds (), old_limit);
				return ret;
			}
		}
	}
	printf("readb: unknow error!\n");
	return 0xFF;
}

static inline unsigned long breadl(unsigned long addr)
{
	void * virtualAddress = NULL;
	unsigned long pageaddr = 0;
	unsigned long size = 0x1000;
	unsigned long offset = 0;
	unsigned long value = 0;

	pageaddr = addr & 0xFFFFF000;
	offset = addr & 0x00000FFF;

	if (__djgpp_map_physical_memory(virtualAddress, size, pageaddr) == 0)
	{
		value = *((unsigned long *)((unsigned long)virtualAddress + offset));
		return value;
	}
	return 0xFF;
}

inline void writeb(unsigned char value, unsigned long addr)
{
	unsigned long new_limit = 0xFFFFFFFF;
	unsigned long old_limit = 0;

	old_limit = __dpmi_get_segment_limit(_my_ds ());

	if (__dpmi_set_segment_limit (_my_ds (), new_limit) == 0)
	{
		if (__dpmi_get_segment_limit (_my_ds ()) != new_limit)
		{
			/* The DPMI host ignored the call.  Fail.  */
			//return 0xFF;
			printf("writeb: Set segment limit error!\n");
		}
		else
		{
			if (__djgpp_nearptr_enable())
			{
				unsigned char *phy_addr = (unsigned char *)(__djgpp_conventional_base + addr);
				*phy_addr = value;
				__djgpp_nearptr_disable();
				__dpmi_set_segment_limit (_my_ds (), old_limit);
			}
		}
	}
	//return 0xFF;
	printf("writeb: unknow error!\n");
}

inline void bwriteb(unsigned char value, unsigned long addr)
{
	void * virtualAddress = NULL;
	unsigned long    pageaddr = 0;
	unsigned long    size = 0x1000;
	unsigned long    offset = 0;
	unsigned char * address = NULL;

	pageaddr = addr & 0xFFFFF000;
	offset = addr & 0x00000FFF;

	if (__djgpp_map_physical_memory(virtualAddress, size, pageaddr) == 0)
	{
		address = (unsigned char *)((unsigned long)virtualAddress + offset);
		*address =  value;
	}
}

inline void writew(unsigned short value, unsigned long addr)
{
	unsigned long new_limit = 0xFFFFFFFF;
	unsigned long old_limit = 0;

	old_limit = __dpmi_get_segment_limit(_my_ds ());

	if (__dpmi_set_segment_limit (_my_ds (), new_limit) == 0)
	{
		if (__dpmi_get_segment_limit (_my_ds ()) != new_limit)
		{
			/* The DPMI host ignored the call.  Fail.  */
			//return 0xFF;
			printf("writew: Set segment limit error!\n");
		}
		else
		{
			if (__djgpp_nearptr_enable())
			{
				unsigned short *phy_addr = (unsigned short *)(__djgpp_conventional_base + addr);
				*phy_addr = value;
				__djgpp_nearptr_disable();
				__dpmi_set_segment_limit (_my_ds (), old_limit);
			}
		}
	}
	//return 0xFF;
	printf("writew: unknow error!\n");
}

inline void bwritew(unsigned short value, unsigned long addr)
{
	void * virtualAddress = NULL;
	unsigned long    pageaddr = 0;
	unsigned long    size = 0x1000;
	unsigned long    offset = 0;
	unsigned short * address = NULL;

	pageaddr = addr & 0xFFFFF000;
	offset = addr & 0x00000FFF;

	if (__djgpp_map_physical_memory(virtualAddress, size, pageaddr) == 0)
	{
		address = (unsigned short *)((unsigned long)virtualAddress + offset);
		*address =  value;
	}
}

inline void writel(unsigned long value, unsigned long addr)
{
	unsigned long new_limit = 0xFFFFFFFF;
	unsigned long old_limit = 0;

	old_limit = __dpmi_get_segment_limit(_my_ds ());

	if (__dpmi_set_segment_limit (_my_ds (), new_limit) == 0)
	{
		if (__dpmi_get_segment_limit (_my_ds ()) != new_limit)
		{
			/* The DPMI host ignored the call.  Fail.  */
			//return 0xFF;
			printf("writel: Set segment limit error!\n");
		}
		else
		{
			if (__djgpp_nearptr_enable())
			{
				unsigned long *phy_addr = (unsigned long *)(__djgpp_conventional_base + addr);
				*phy_addr = value;
				__djgpp_nearptr_disable();
				__dpmi_set_segment_limit (_my_ds (), old_limit);
			}
		}
	}
	//return 0xFF;
	printf("writel: unknow error!\n");
}

inline void bwritel(unsigned long value, unsigned long addr)
{
	void * virtualAddress = NULL;
	unsigned long    pageaddr = 0;
	unsigned long    size = 0x1000;
	unsigned long    offset = 0;
	unsigned long * address = NULL;

	pageaddr = addr & 0xFFFFF000;
	offset = addr & 0x00000FFF;

	if (__djgpp_map_physical_memory(virtualAddress, size, pageaddr) == 0)
	{
		address = (unsigned long *)((unsigned long)virtualAddress + offset);
		*address =  value;
	}
}

UINT pci_bus_read_config_dword(UCHAR bus, UCHAR devfn, UCHAR offset, ULONG * pvalue)
{
	ULONG    addr = 0x80000000;

	if (pvalue == NULL)
		return 1;        // Fix Here
	offset = offset/4 * 4;
	addr = addr + (bus << 16) + (devfn << 8) + offset;
	outl(addr, QIO_PCI_ADDR_PORT);
	*pvalue = inl(QIO_PCI_DATA_PORT);
	return 0;
}

UINT pci_bus_read_config_byte(UCHAR bus, UCHAR devfn, UCHAR offset, UCHAR * pvalue)
{
	ULONG    value =0;
	UINT ret=0;
	UCHAR    longoffset= offset/4 * 4;
	UCHAR    index = offset - longoffset;

	if (pvalue == NULL)
		return 1;        // Fix Here

	ret = pci_bus_read_config_dword(bus, devfn, longoffset, &value);
	*pvalue = (UCHAR) ((value >> (index*8)) & 0xFF);
	return 0;
}
UINT pci_bus_read_config_word(UCHAR bus, UCHAR devfn, UCHAR offset, USHORT * pvalue)
{
	ULONG    value =0;
	UINT ret=0;
	UCHAR    longoffset= offset/4 * 4;
	UCHAR    index = offset - longoffset;

	if (pvalue == NULL)
		return 1;        // Fix Here

	ret = pci_bus_read_config_dword(bus, devfn, longoffset, &value);
	*pvalue = (USHORT) ((value >> (index*8)) & 0xFFFF);
	if (index >= 3)
	{
		ret = pci_bus_read_config_dword(bus, devfn, longoffset+4, &value);
		*pvalue += (USHORT) ((value  & 0xFF) <<8);
	}

	return 0;
}

UINT pci_bus_write_config_dword(UCHAR bus, UCHAR devfn, UCHAR offset, ULONG value)
{
	ULONG    addr = 0x80000000;

	offset = offset/4 * 4;
	addr = addr + (bus << 16) + (devfn << 8) + offset;
	outl(addr, QIO_PCI_ADDR_PORT);
	outl(value, QIO_PCI_DATA_PORT);
	return 0;
}

UINT pci_bus_write_config_byte(UCHAR bus, UCHAR devfn, UCHAR offset, UCHAR value)
{
	ULONG    temp_value =0;
	UINT ret=0;
	UCHAR    longoffset= offset/4 * 4;
	UCHAR    index = offset - longoffset;
	UCHAR* pvalue = (UCHAR*)&temp_value;

	ret = pci_bus_read_config_dword(bus, devfn, longoffset, &temp_value);
	*(pvalue + index ) = value;
	ret = pci_bus_write_config_dword(bus, devfn, longoffset, temp_value);

	return 0;
}
UINT pci_bus_write_config_word(UCHAR bus, UCHAR devfn, UCHAR offset, USHORT  value)
{
	ULONG    temp_value =0;
	UINT ret=0;
	UCHAR    longoffset= offset/4 * 4;
	UCHAR    index = offset - longoffset;
	UCHAR* pvalue = (UCHAR*)&temp_value;
	if (index <3)
	{
		ret = pci_bus_read_config_dword(bus, devfn, longoffset, &temp_value);
		*(USHORT *)(pvalue + index ) = value;
		ret = pci_bus_write_config_dword(bus, devfn, longoffset, temp_value);
	}
	else
	{
		ret = pci_bus_read_config_dword(bus, devfn, longoffset, &temp_value);
		*(pvalue + index ) =(UCHAR) (value & 0xFF);
		ret = pci_bus_write_config_dword(bus, devfn, longoffset, temp_value);
		ret = pci_bus_read_config_dword(bus, devfn, longoffset+4, &temp_value);
		*pvalue=(UCHAR) (value >> 8);
		ret = pci_bus_write_config_dword(bus, devfn, longoffset+4, temp_value);
	}

	return 0;
}

static inline void readmsr(unsigned long ecx, unsigned long* eax, unsigned long* edx)    
{
	asm volatile("rdmsr" : "=a"(*eax), "=d"(*edx) : "c"(ecx) );
}

static inline void writemsr(unsigned long ecx, unsigned long eax, unsigned long edx)           
{
	asm volatile("wrmsr"  : : "c"(ecx), "a"(eax), "d"(edx));
}   

inline static USHORT GetPMBase(void)
{
	UINT	ret = 0;
	USHORT	base =0;
	ret = pci_bus_read_config_word(0, 0xF8, 0x40, &base);
	base &= 0xFF80;
	return base;
}

inline static unsigned long GetPCIeBase(void)
{
	unsigned int	ret = 0;
	unsigned long	base =0;
	ret = pci_bus_read_config_dword(0, 0x80, 0x64, &base);
	base &= 0x00FFF000;
	base = base << 28;
	return 0xE0000000;

	//return 0xE0000000;    //Commonly the PCIe base is here, but we read it from HECBASE
}

UINT pcie_bus_read_config_dword(UCHAR bus, UCHAR devfn, USHORT offset, ULONG * pvalue)
{
	ULONG    base = 0;
	ULONG    addr = 0;

	if (pvalue == NULL)
		return 1;        // Fix Here
	base = GetPCIeBase();
	addr = base + (bus << 20) + (devfn << 12) + offset;
	*pvalue = breadl(addr);
    
	return 0;
}

UINT pcie_bus_read_config_word(UCHAR bus, UCHAR devfn, USHORT offset, USHORT * pvalue)
{
	ULONG    base = 0;
	ULONG    addr = 0;

	if (pvalue == NULL)
		return 1;        // Fix Here
	base = GetPCIeBase();
	addr = base + (bus << 20) + (devfn << 12) + offset;
	*pvalue = breadw(addr);
    
	return 0;
}

UINT pcie_bus_read_config_byte(UCHAR bus, UCHAR devfn, USHORT offset, UCHAR * pvalue)
{
	ULONG    base = 0;
	ULONG    addr = 0;

	if (pvalue == NULL)
		return 1;        // Fix Here
	base = GetPCIeBase();
	addr = base + (bus << 20) + (devfn << 12) + offset;
	*pvalue = breadb(addr);

	return 0;
}

UINT pcie_bus_write_config_dword(UCHAR bus, UCHAR devfn, USHORT offset, ULONG value)
{
	ULONG    base = 0;
	ULONG    addr = 0;

	base = GetPCIeBase();
	addr = base + (bus << 20) + (devfn << 12) + offset;
	bwritel(value, addr);
	return 0;
}

UINT pcie_bus_write_config_word(UCHAR bus, UCHAR devfn, USHORT offset, USHORT value)
{
	ULONG    base = 0;
	ULONG    addr = 0;

	base = GetPCIeBase();
	addr = base + (bus << 20) + (devfn << 12) + offset;
	bwritew(value, addr);
	return 0;
}

UINT pcie_bus_write_config_byte(UCHAR bus, UCHAR devfn, USHORT offset, UCHAR value)
{
	ULONG    base = 0;
	ULONG    addr = 0;

	base = GetPCIeBase();
	addr = base + (bus << 20) + (devfn << 12) + offset;
	bwriteb(value, addr);
	return 0;
}

#define	QIO_CMOS_INDEX_PORT	0x70
#define	QIO_CMOS_DATA_PORT	0x71
#define	QIO_CMOS_EXT_INDEX_PORT	0x72
#define	QIO_CMOS_EXT_DATA_PORT	0x73

UINT cmos_read(UCHAR index, UCHAR * pvalue)
{
	// Read the first 0x00 ~ 0x7f index value of CMOS

	if (index < 0x80)
	{
		outb(index, QIO_CMOS_INDEX_PORT);
		*pvalue = inb(QIO_CMOS_DATA_PORT);
	}
	else
	{
		outb(index, QIO_CMOS_EXT_INDEX_PORT);
		*pvalue = inb(QIO_CMOS_EXT_DATA_PORT);
	}

	return 0 ;
}

UINT cmos_write(UCHAR index, UCHAR value)
{
	if (index < 0x80)
	{
		outb(index, QIO_CMOS_INDEX_PORT);
		outb(value, QIO_CMOS_DATA_PORT);
	}
	else
	{
		outb(index, QIO_CMOS_EXT_INDEX_PORT);
		outb(value, QIO_CMOS_EXT_DATA_PORT);
	}

	return 0 ;
}

/*++
*************************************************************************************************
DJGPP API exported by freevanx

**** Read IO port
static inline unsigned char inb(int port);
static inline unsigned short inw(int port);
static inline unsigned int inl(int port);

**** Write IO port
static inline void outb(unsigned char value, int port);
static inline void outw(unsigned short value, int port);
static inline void outl(unsigned long int, int port);

**** Read/write memory below 1MB
static inline unsigned char readb(unsigned long addr);
static inline unsigned short readw(unsigned long addr);
static inline unsigned long readl(unsigned long addr);

inline void writeb(unsigned char value, unsigned long addr);
inline void writew(unsigned short value, unsigned long addr);
inline void writel(unsigned long value, unsigned long addr);

**** Read/write memory/MMIO above 1MB
static inline unsigned char breadb( unsigned long addr);
static inline unsigned short breadw( unsigned long addr);
static inline unsigned long breadl( unsigned long addr);

inline void bwriteb(unsigned char value, unsigned long addr);
inline void bwritew(unsigned short value, unsigned long addr);
inline void bwritel(unsigned long value, unsigned long addr);

**** Read PCI
UINT pci_bus_read_config_dword(UCHAR bus, UCHAR devfn, UCHAR offset, ULONG * pvalue);
UINT pci_bus_read_config_byte(UCHAR bus, UCHAR devfn, UCHAR offset, UCHAR * pvalue);
UINT pci_bus_read_config_word(UCHAR bus, UCHAR devfn, UCHAR offset, USHORT * pvalue);

**** Write PCI
UINT pci_bus_write_config_dword(UCHAR bus, UCHAR devfn, UCHAR offset, ULONG  value);
UINT pci_bus_write_config_byte(UCHAR bus, UCHAR devfn, UCHAR offset, UCHAR  value);
UINT pci_bus_write_config_word(UCHAR bus, UCHAR devfn, UCHAR offset, USHORT  value);

**** Read/write CPU MSR
static inline void readmsr(unsigned long ecx, unsigned long* eax, unsigned long* edx);
static inline void writemsr(unsigned long ecx, unsigned long eax, unsigned long edx);

**** Read/write PCIe(care the PCIe base address, it should be correct)
UINT pcie_bus_read_config_dword(UCHAR bus, UCHAR devfn, USHORT offset, ULONG * pvalue);
UINT pcie_bus_read_config_word(UCHAR bus, UCHAR devfn, USHORT offset, USHORT * pvalue);
UINT pcie_bus_read_config_byte(UCHAR bus, UCHAR devfn, USHORT offset, UCHAR * pvalue);

UINT pcie_bus_write_config_dword(UCHAR bus, UCHAR devfn, USHORT offset, ULONG value);
UINT pcie_bus_write_config_word(UCHAR bus, UCHAR devfn, USHORT offset, USHORT value);
UINT pcie_bus_write_config_byte(UCHAR bus, UCHAR devfn, USHORT offset, UCHAR value);

**** Read/write CMOS
UINT cmos_read(UCHAR index, UCHAR * pvalue);
UINT cmos_write(UCHAR index, UCHAR value);
--************************************************************************************************/


char  gMsg[] = "lychee Project (c) freevanx All Rights Reserved!\n" \
               "QuikIO hardware access library!\n" \
               "ms delay utility for DOS, max delay input value is 1199879 = 19.99 min\n" \
               "    Build by DJGPP";

int verbose =0;

int main(void)
{
    return 0;
}
